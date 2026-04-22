from PyQt5.QtWidgets import QWidget, QVBoxLayout, QLabel, QPushButton, QLineEdit, QGridLayout
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QRegExpValidator
from PyQt5.QtCore import QRegExp

class LoginScreen(QWidget):
    """
    A PyQt5-based login screen that provides username and password input fields,
    a login button, and validation logic.
    """

    def __init__(self, main_window):
        """
        Initializes the LoginScreen and sets up the UI.

        Parameters:
        main_window (QWidget): The main application window that will be updated upon successful login.

        Returns:
        None
        """
        super().__init__()
        self.main_window = main_window  # Reference to the main application window
        self.init_ui()  # Initialize the UI components

    def init_ui(self):
        """
        Creates and configures the UI layout, including labels, input fields, buttons, and a message label.

        Parameters:
        None

        Returns:
        None
        """
        # Create a vertical layout for the login screen
        layout = QVBoxLayout()
        layout.setSpacing(10)
        layout.setAlignment(Qt.AlignCenter)

        # Add spacing before the login components
        layout.addStretch(1)

        # Add a header label
        layout.addWidget(self.create_widget('label', "","Cubex® System Ready", "#87CEEB", 28, True, 30))

        # Create a grid layout for the form (Username and Password fields)
        form_layout = QGridLayout()
        form_layout.setSpacing(20)

        # Create and add the username input field
        self.username_input = self.create_widget('input', "admin", "Enter Username", width=300)
        # Create the username input
        self.username_input = self.create_widget('input', "admin", "Enter Username", width=300)

        # Enforce lowercase letters only
        regexp = QRegExp("[a-z0-9_]+")  # Allow lowercase letters, numbers, and underscore
        validator = QRegExpValidator(regexp)
        self.username_input.setValidator(validator)

        # Optional: automatically convert uppercase to lowercase while typing
        def to_lowercase(text):
            cursor_pos = self.username_input.cursorPosition()
            self.username_input.setText(text.lower())
            self.username_input.setCursorPosition(cursor_pos)

        self.username_input.textChanged.connect(to_lowercase)
        
        self.password_input = self.create_widget('input', "Enter Password","123456", is_password=True, width=300)

        # Add username label and input field
        self.password_input = self.create_widget('input', "123456", "Enter Password", is_password=True, width=300)
        form_layout.addWidget(self.username_input, 0, 1, Qt.AlignCenter)

        layout.setSpacing(50)

        # Add password label and input field
        form_layout.addWidget(self.create_widget('label', "","Password:", "white", 18, "bold"), 1, 0, Qt.AlignCenter)
        form_layout.addWidget(self.password_input, 1, 1, Qt.AlignCenter)

        # Add form layout to the main layout
        layout.addLayout(form_layout)

        # Create and add the login button
        login_button = self.create_widget('button', "Login", click_handler=self.validate_login, fixed_size=(300, 45))

        # Apply styles to the login button
        login_button.setStyleSheet("""
            QPushButton { background-color: #66ccff; color: black; font-size: 16px; border-radius: 10px; }
            QPushButton:hover { background-color: #6495ED; }
        """) 
        
        layout.addWidget(login_button, alignment=Qt.AlignmentFlag.AlignRight)        

        # Add spacing after the login button
        layout.addSpacing(10)

        # Create a label to display messages (success/failure)
        self.label_message = QLabel("")
        self.label_message.setStyleSheet("color: white; font-size: 14px;")

        # Add the message label to the layout
        layout.addWidget(self.label_message, alignment=Qt.AlignCenter)

        # Add final spacing at the bottom
        layout.addStretch(3)

        # Set the layout for the login screen
        self.setLayout(layout)

    def validate_login(self):
        """
        Validates the username and password entered by the user.
        
        If the username is either "admin" or "testtech" and the password matches "123456",
        it updates the main window with the username and transitions to the main screen.
        Otherwise, it displays an error message.

        Parameters:
        None

        Returns:
        None
        """
        username = self.username_input.text()  # Get the entered username
        password = self.password_input.text()  # Get the entered password
        
        if not username or not password:
            self.label_message.setText("✖ Username and password cannot be empty!")
            self.label_message.setStyleSheet("color: red; font-size: 16px; font-weight: bold;")
            return
    
        # Check if username is either "admin" or "testtech" AND password is correct
        if username.lower() in ["admin", "testtech"] and password == "123456":
            # Set success message style
            self.label_message.setText("")
            self.label_message.setStyleSheet("color: #4CAF50; font-size: 16px; font-weight: bold;")
            self.main_window.set_username(username)  # Update main window with username
            self.main_window.show_main_screens()  # Transition to the main application screens
        else:
            # Display an error message
            self.label_message.setText("✖ Invalid username or password!")
            self.label_message.setStyleSheet("color: red; font-size: 16px; font-weight: bold;")
            
    def create_widget(self, widget_type, text=None, placeholder_text = None,color=None, size=None, bold=False, padding=0, width=None, is_password=False, click_handler=None, fixed_size=None):
        """
        Dynamically creates and returns a widget (QLabel, QLineEdit, or QPushButton) based on the provided parameters.

        Parameters:
        widget_type (str): Type of widget to create ('label', 'input', or 'button').
        text (str, optional): Text to display on the widget (default: None).
        color (str, optional): Text color for labels (default: None).
        size (int, optional): Font size for labels (default: None).
        bold (bool, optional): Whether to apply bold formatting to labels (default: False).
        padding (int, optional): Bottom padding for labels (default: 0).
        width (int, optional): Width of input fields (default: None).
        is_password (bool, optional): Whether the input field should hide text (default: False).
        click_handler (function, optional): Function to call when the button is clicked (default: None).
        fixed_size (tuple, optional): Fixed width and height for buttons (default: None).

        Returns:
        QWidget: The created widget (QLabel, QLineEdit, or QPushButton).
        """
        # Dictionary mapping widget types to creation functions
        widget_map = {
            'label': lambda: QLabel(text),
            'input': lambda: self.input_field(text, placeholder_text, is_password),
            'button': lambda: QPushButton(text)
        }

        # Get the corresponding widget creation function
        widget = widget_map.get(widget_type, lambda: None)()

        # If an invalid widget type is provided, return None
        if not widget:
            return None

        # Apply properties for QLabel
        if widget_type == 'label':
            widget.setAlignment(Qt.AlignCenter)
            widget.setStyleSheet(f"color: {color}; font-size: {size}px; {'font-weight: bold;' if bold else ''} padding-bottom: {padding}px; bold")

        # Apply properties for QLineEdit (input field)
        if widget_type == 'input' and width:
            widget.setFixedWidth(width)

        # Apply properties for QPushButton
        if widget_type == 'button':
            widget.setStyleSheet(""" 
                QPushButton { background: #00ADEF; color: white; font-size: 14px; padding: 10px; border-radius: 10px; bold }
                QPushButton:hover { background: #3E4452; }
                QPushButton:pressed { background: #0088CC; }
            """)
            if fixed_size:
                widget.setFixedSize(*fixed_size)  # Set fixed button size
            if click_handler:
                widget.clicked.connect(click_handler)  # Connect button click event to handler

        return widget  # Return the created widget

    def input_field(self, text, placeholder_text, is_password=False):
        """
        Creates and returns a QLineEdit input field with a default value and a placeholder.

        Parameters:
        text (str): Default text to display inside the input field.
        placeholder_text (str): Placeholder text to display when empty.
        is_password (bool, optional): Whether the input field should hide text (default: False).

        Returns:
        QLineEdit: The created input field.
        """
        input_widget = QLineEdit()
        input_widget.setPlaceholderText(placeholder_text)  # Set placeholder text
        #input_widget.setText(text)  # Set default text
        input_widget.setEchoMode(QLineEdit.Password if is_password else QLineEdit.Normal)  # Set password mode if required
        input_widget.setStyleSheet(
            "background-color: white; color: black; padding: 15px; border-radius: 15px; font-size: 16px;"
        )
        return input_widget

