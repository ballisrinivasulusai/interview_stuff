#!/bin/bash

# Detect the OS
OS=$(uname)

# Set the application name
APP_NAME="Cubex_V1.8.3_MVP"

# Clean up previous build (if necessary)
echo "Cleaning up previous build..."
rm -rf build dist *.spec

# Check if virtual environment exists, otherwise create it
if [ ! -d "venv" ]; then
    echo "Virtual environment not found. Creating new virtual environment..."
    python3 -m venv venv
else
    echo "Virtual environment already exists. Skipping creation."
fi

# Ensure proper permissions for the virtual environment
echo "Setting permissions for the virtual environment..."
chmod -R +x venv/bin/*

# Activate the virtual environment
echo "Activating virtual environment..."
source venv/bin/activate

# Install dependencies from requirements.txt
echo "Installing dependencies from requirements.txt..."
pip install --upgrade pip
pip install -r requirements.txt

# Build the executable with PyInstaller
echo "Building application with PyInstaller..."
if [[ "$OS" == "Linux" || "$OS" == "Darwin" ]]; then
    # Ensure macOS creates an .app bundle
    pyinstaller --windowed --name="$APP_NAME" app.py
elif [[ "$OS" == "MINGW"* || "$OS" == "CYGWIN"* || "$OS" == "Windows_NT"* ]]; then
    # For Windows, build the .exe
    pyinstaller --onefile --name="$APP_NAME" app.py
else
    echo "Unsupported OS for building executable"
    exit 1
fi

# Check if build succeeded
if [ $? -eq 0 ]; then
    echo "Build successful!"
else
    echo "Build failed. Please check the logs."
    exit 1
fi

# Create installer package based on OS
if [ "$OS" == "Darwin" ]; then
    echo "Creating .dmg file..."

    # Define directories (Use dist instead of build)
    PROJECT_DIR=$(pwd)
    BUILD_DIR="$PROJECT_DIR/dist"
    APP_BUNDLE="$BUILD_DIR/$APP_NAME.app"
    DMG_FILE="$BUILD_DIR/$APP_NAME.dmg"

    # Check if the app bundle exists
    if [ -d "$APP_BUNDLE" ]; then
        # Create the .dmg file using hdiutil
        hdiutil create -volname "$APP_NAME" -srcfolder "$APP_BUNDLE" -ov -format UDZO "$DMG_FILE"

        # Check if the .dmg file was created
        if [ -f "$DMG_FILE" ]; then
            echo ".dmg file created successfully at $DMG_FILE"
        else
            echo "Failed to create .dmg file."
            exit 1
        fi
    else
        echo "App bundle not found in dist/. Build might have failed."
        exit 1
    fi
elif [ "$OS" == "Linux" ]; then
    echo "Creating .deb package..."

    # Define directories
    PROJECT_DIR=$(pwd)
    BUILD_DIR="$PROJECT_DIR/dist"
    APP_BINARY="$BUILD_DIR/$APP_NAME"

    # Check if the binary exists
    if [ -f "$APP_BINARY" ]; then
        # Create a .deb package using dpkg
        mkdir -p "$BUILD_DIR/DEBIAN"
        echo "Package: $APP_NAME" > "$BUILD_DIR/DEBIAN/control"
        echo "Version: 1.2" >> "$BUILD_DIR/DEBIAN/control"
        echo "Section: base" >> "$BUILD_DIR/DEBIAN/control"
        echo "Priority: optional" >> "$BUILD_DIR/DEBIAN/control"
        echo "Architecture: all" >> "$BUILD_DIR/DEBIAN/control"
        echo "Maintainer: Your Name <your.email@example.com>" >> "$BUILD_DIR/DEBIAN/control"
        echo "Description: $APP_NAME" >> "$BUILD_DIR/DEBIAN/control"
        dpkg-deb --build "$BUILD_DIR" "$BUILD_DIR/$APP_NAME.deb"

        # Check if the .deb file was created
        if [ -f "$BUILD_DIR/$APP_NAME.deb" ]; then
            echo ".deb file created successfully at $BUILD_DIR/$APP_NAME.deb"
        else
            echo "Failed to create .deb file."
            exit 1
        fi
    else
        echo "Binary not found in dist/. Build might have failed."
        exit 1
    fi
elif [[ "$OS" == "MINGW"* || "$OS" == "CYGWIN"* || "$OS" == "Windows_NT"* ]]; then
    echo "Creating .msi installer..."

    # Define directories
    PROJECT_DIR=$(pwd)
    BUILD_DIR="$PROJECT_DIR/dist"
    APP_BINARY="$BUILD_DIR/$APP_NAME.exe"

    # Check if the binary exists
    if [ -f "$APP_BINARY" ]; then
        # Create a .msi installer using WiX Toolset
        candle -out "$BUILD_DIR/$APP_NAME.wixobj" "$PROJECT_DIR/$APP_NAME.wxs"
        light -out "$BUILD_DIR/$APP_NAME.msi" "$BUILD_DIR/$APP_NAME.wixobj"

        # Check if the .msi file was created
        if [ -f "$BUILD_DIR/$APP_NAME.msi" ]; then
            echo ".msi file created successfully at $BUILD_DIR/$APP_NAME.msi"
        else
            echo "Failed to create .msi file."
            exit 1
        fi
    else
        echo "Binary not found in dist/. Build might have failed."
        exit 1
    fi
else
    echo "Unsupported OS for creating installer package"
    exit 1
fi

# Done
echo "Build and installer creation process completed."
