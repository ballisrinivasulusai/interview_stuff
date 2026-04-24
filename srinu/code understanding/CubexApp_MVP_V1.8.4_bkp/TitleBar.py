from PyQt5.QtWidgets import QWidget, QLabel, QPushButton, QVBoxLayout, QHBoxLayout
from PyQt5.QtCore import Qt

class TitleBar(QWidget):
    """
    Custom title bar widget that displays the username, a title, and a logout button.
    """
    
    def __init__(self, main_window, username, title):
        """
        Initializes the TitleBar widget.
        
        :param main_window: The main application window that handles logout functionality.
        :type main_window: QWidget
        :param username: The name of the currently logged-in user.
        :type username: str
        :param title: The title text to be displayed in the title bar.
        :type title: str
        :returns: None
        """
        super().__init__()
        self.main_window = main_window
        self.username = username
        self.title = title
        self.init_ui()
    
    def init_ui(self):
        """
        Sets up the UI layout for the title bar, including labels and a logout button.
        
        :returns: None
        """
        self.username_label = QLabel(f"USER: {self.username}")
        self.username_label.setStyleSheet("font-size: 18px; color: white; font-weight: bold; padding: 5px; background-color: #242746;")
        
        self.title_label = QLabel(self.title)
        self.title_label.setStyleSheet("font-size: 22px; color: #87CEEB; font-weight: bold; padding: 5px; background-color: #242746;")
        
        self.logout_button = QPushButton("Logout")
        self.logout_button.clicked.connect(self.main_window.logout)
        self.logout_button.setStyleSheet("""
            QPushButton { background: #FF6347; color: white; font-size: 14px; padding: 5px; border-radius: 5px; }
            QPushButton:hover { background: #FF4500; }
        """)
        self.logout_button.setFixedSize(100, 30)

        top_layout = QHBoxLayout()
        top_layout.addWidget(self.username_label)
        top_layout.addStretch()
        top_layout.addWidget(self.title_label)
        top_layout.addStretch()
        top_layout.addWidget(self.logout_button)
        top_layout.setAlignment(Qt.AlignTop)
        
        main_layout = QVBoxLayout()
        main_layout.addLayout(top_layout)
        main_layout.addStretch(1)
        
        self.setLayout(main_layout)
        self.setFixedHeight(1000)  
