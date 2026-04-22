#!/usr/bin/env python3
# sfc5xx_api_compat.py
#
# SFC5xxx wrapper exposing SFC6-style method names:
#   - setpoint_percentage(id, percent)
#   - read_average_value(id, number_of_values=5)
#   - measure_raw_flow(id)
#   - set_calibration(id, cal)   <-- dummy (no-op unless supported)
#   - connect(id, baudrate=..., additional_response_time=..., slave_address=0)
#   - disconnect(id)
#
# Internals (your working logic):
#   sensirion_tinkerforge_driver.ShdlcSerialPort
#   -> adapter adds .description/.name
#   -> sensirion_tinkerforge_driver.ShdlcConnection
#   -> sensirion_shdlc_sfc5xxx.Sfc5xxxShdlcDevice

import os
import sys
import time
from sensirion_tinkerforge_driver import ShdlcSerialPort, ShdlcConnection
from sensirion_shdlc_sfc5xxx.device import Sfc5xxxShdlcDevice
# (Optional) add venv site-packages (Windows + Linux/mac)
venv = os.getenv("VIRTUAL_ENV")
if venv:
    for sp in (
        os.path.join(venv, "Lib", "site-packages"),  # Windows
        os.path.join(venv, "lib", f"python{sys.version_info.major}.{sys.version_info.minor}", "site-packages"),  # Linux/mac
    ):
        if os.path.isdir(sp) and sp not in sys.path:
            sys.path.append(sp)




# ---------- Tiny adapter (as in your working code) ----------
class _PortAdapter:
    """
    Minimal adapter to satisfy the port interface expected by ShdlcConnection.
    - Forwards transceive() and close() to the underlying ShdlcSerialPort
    - Provides .description and .name attributes used by connection/logging
    """
    def __init__(self, inner_port, desc=None, tag="TF"):
        self._inner = inner_port
        default_desc = f"{tag}:{getattr(inner_port, 'port', 'uri')}@{getattr(inner_port, 'baudrate', '')}"
        self._description = desc or getattr(inner_port, "description", default_desc)

    @property
    def description(self):
        return self._description

    @property
    def name(self):
        return self._description

    def transceive(self, slave_address, command_id, data, response_timeout):
        return self._inner.transceive(slave_address, command_id, data, response_timeout)

    def close(self):
        try:
            self._inner.close()
        except Exception:
            pass


# ---------- SFC5 wrapper with SFC6-style API ----------
class _SFC5Handle:
    def __init__(self, tf_uri, baudrate, extra_resp_time, slave_address):
        self.tf_uri = tf_uri
        self.baudrate = int(baudrate)
        self.extra = float(extra_resp_time)
        self.addr = int(slave_address)
        self.tf_port = None
        self.conn = None
        self.dev = None

    def open(self):
        # Open TF-backed SHDLC port
        self.tf_port = ShdlcSerialPort(
            port=self.tf_uri,
            baudrate=self.baudrate,
            additional_response_time=self.extra,
        )
        # Wrap for connection ctor (expects .description/.name)
        pad = _PortAdapter(self.tf_port)
        # Build connection
        self.conn = ShdlcConnection(pad)
        # Device
        self.dev = Sfc5xxxShdlcDevice(connection=self.conn, slave_address=self.addr)
        time.sleep(0.1)

    def close(self):
        try:
            if self.conn and hasattr(self.conn, "port") and hasattr(self.conn.port, "close"):
                self.conn.port.close()
        except Exception:
            pass
        try:
            if self.tf_port:
                self.tf_port.close()
        except Exception:
            pass
        self.tf_port = None
        self.conn = None
        self.dev = None


class mfc_sfc5:
    """
    Compatibility layer exposing SFC6-style method names for SFC5xxx hardware.
    Hold references by 'id' which is the TF URI string (e.g. "localhost/4223/28Du").
    """
    h = {}  # id -> _SFC5Handle

    # --- lifecycle ---
    @classmethod
    def connect(cls, serial_port, baudrate=115200, additional_response_time=2.0, slave_address=0):
        """
        serial_port: "host/port/UID" (e.g., "localhost/4223/28Du")
        Returns None on success, or error string on failure.
        """
        if not serial_port:
            return "Device ID (serial_port) required."
        if serial_port in cls.h:
            return f'"{serial_port}" already connected.'
        try:
            h = _SFC5Handle(serial_port, baudrate, additional_response_time, slave_address)
            h.open()
            cls.h[serial_port] = h
            return None
        except Exception as e:
            try:
                h.close()
            except Exception:
                pass
            return f"Connect failed: {e}"

    @classmethod
    def disconnect(cls, id):
        h = cls.h.get(id)
        if not h:
            return
        h.close()
        del cls.h[id]

    @classmethod
    def _get_handle(cls, id) -> _SFC5Handle:
        if not id or id not in cls.h:
            raise ValueError(f'Invalid ID "{id}". Currently connected: {list(cls.h.keys())}')
        return cls.h[id]

    # --- helpers (internal) ---
    @staticmethod
    def _read_measured_value(dev):
        # Prefer PHYSICAL scaling (slm), fallback to default signature
        try:
            from sensirion_shdlc_sfc5xxx.definitions import Sfc5xxxScaling
            return dev.read_measured_value(scaling=Sfc5xxxScaling.PHYSICAL)
        except Exception:
            return dev.read_measured_value()

    # --- SFC6-style public API ---
    @classmethod
    def setpoint_percentage(cls, id, percentage):
        """
        Set setpoint to <percentage>% of full-scale (normalized units).
        Returns 1 on success, 0 on failure.
        """
        h = cls._get_handle(id)
        try:
            p = float(percentage)
        except Exception:
            print(f"❌ ERROR: Invalid percentage '{percentage}'")
            return 0
        p = max(0.0, min(100.0, p))
        normalized = p / 100.0
        # Prefer explicit NORMALIZED scaling
        try:
            from sensirion_shdlc_sfc5xxx.definitions import Sfc5xxxScaling
            h.dev.set_setpoint(normalized, scaling=Sfc5xxxScaling.NORMALIZED)
        except Exception:
            # Fallback: default set_setpoint() accepts normalized in many lib versions
            h.dev.set_setpoint(normalized)
        time.sleep(0.2)  # brief settle
        return 1

    @classmethod
    def read_average_value(cls, id, number_of_values=5):
        """
        Average N measured values (best-effort PHYSICAL units in slm).
        Mirrors the SFC6 read_averaged_measured_value() behavior.
        """
        h = cls._get_handle(id)
        try:
            n = max(1, int(number_of_values))
        except Exception:
            n = 5
        total = 0.0
        count = 0
        last = None
        for _ in range(n):
            try:
                val = cls._read_measured_value(h.dev)
                last = val
                total += float(val)
                count += 1
            except Exception:
                pass
            time.sleep(0.1)
        if count == 0:
            return last
        return total / count

    @classmethod
    def measure_raw_flow(cls, id):
        """
        Best-effort 'raw flow' for SFC5:
        - If the library exposes dev.measure_raw_flow(), use it.
        - Otherwise, return the current measured (physical) flow as a proxy.
        """
        h = cls._get_handle(id)
        try:
            if hasattr(h.dev, "measure_raw_flow"):
                return h.dev.measure_raw_flow()
        except Exception:
            pass
        try:
            return cls._read_measured_value(h.dev)
        except Exception as e:
            return f"(error reading flow: {e})"

    @classmethod
    def set_calibration(cls, id, cal):
        """
        DUMMY for API compatibility with SFC6.
        - If the SFC5 library exposes set_calibration(), call it.
        - Otherwise, no-op. Always returns 1 so existing code proceeds.
        """
        h = cls._get_handle(id)
        try:
            cal_i = int(cal)
        except Exception:
            cal_i = cal
        try:
            if hasattr(h.dev, "set_calibration"):
                h.dev.set_calibration(cal_i)  # may not exist on some SFC5 firmwares
                return 1
        except Exception:
            # Ignore any firmware/library mismatch
            pass
        # No-op dummy
        print(f"[INFO] set_calibration({id}, {cal}) is a no-op on this SFC5 wrapper.")
        return 1

    @classmethod
    def get_product_name(cls, id):
        h = cls._get_handle(id)
        return h.dev.get_product_name()

    @classmethod
    def get_article_code(cls, id):
        h = cls._get_handle(id)
        return h.dev.get_article_code()

    @classmethod
    def get_serial_number(cls, id):
        h = cls._get_handle(id)
        return h.dev.get_serial_number()

    @classmethod
    def get_version(cls, id):
        h = cls._get_handle(id)
        return h.dev.get_version()

    @classmethod
    def product_type(cls, id):
        h = cls._get_handle(id)
        return h.dev.get_product_type()
    
# --------------- Example (optional) ---------------
if __name__ == "__main__":
    serial_port = "localhost/4223/28Du"
    mfc_sfc6 = mfc_sfc5
    err = mfc_sfc6.connect(serial_port, baudrate=115200, additional_response_time=2.0, slave_address=0)
    if err:
        print(err)
        sys.exit(1)

    try:
        print("Product Type:", mfc_sfc6.product_type(serial_port))
        print("Product Name:", mfc_sfc6.get_product_name(serial_port))
        print("Article Code:", mfc_sfc6.get_article_code(serial_port))
        print("Serial Number:", mfc_sfc6.get_serial_number(serial_port))      
        print("Version:",        mfc_sfc6.get_version(serial_port))  

        
        
        # Example usage mirroring your SFC6 calls:
        mfc_sfc6.setpoint_percentage(serial_port, 30)
        print(f'flow = {mfc_sfc6.read_average_value(serial_port, 5)}')
        print(f'Raw Flow = {mfc_sfc6.measure_raw_flow(serial_port)}')

        # Dummy calibration call
        mfc_sfc6.set_calibration(serial_port, 4)

        time.sleep(5)

        mfc_sfc6.setpoint_percentage(serial_port, 50)
        print(f'flow = {mfc_sfc6.read_average_value(serial_port, 5)}')
        print(f'Raw Flow = {mfc_sfc6.measure_raw_flow(serial_port)}')

        time.sleep(5)

        mfc_sfc6.setpoint_percentage(serial_port, 0)
        print(f'flow = {mfc_sfc6.read_average_value(serial_port, 5)}')
        print(f'Raw Flow = {mfc_sfc6.measure_raw_flow(serial_port)}')

    finally:
        mfc_sfc6.disconnect(serial_port)


