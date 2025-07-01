# MediaManager

MediaManager is a student project from the Object-Oriented Programming course at the University of Padua.  
The program helps you organize different types of media (like books, poems, and articles) along with their details, using a simple graphical interface made with Qt.

## What you can do

- Add, edit, and remove media entries
- View details for each item
- The program currently supports:
    - **Books**
    - **Poems**
    - **Academic Articles**
    - **Newspaper Articles**

Books and poems share some information, as do academic and newspaper articles.

The code uses some common programming techniques (like Visitor and Observer patterns) to make it easy to extend with new media types or details in the future.

## How to run

1. **Build the app** using Qt Creator or running `qmake` and `make` (`.pro` is included)
2. **Important:**  
   Before running the app for the first time, **copy the `media` and `saves` folders** (included in the project) into the same folder where the executable is created.  
   This is needed for the program to find images and to load/save data.
3. Start the app (`MediaManager` or `MediaManager.app`).

