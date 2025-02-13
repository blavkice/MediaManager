MediaManager is the project for the Object-Oriented course at UNIPD:
it consists of a simple manager capable of handling different types of media files (images, videos, audio files) and their metadata.
Crucial features of the project are the use of design patterns (Visitor and Observer) and the implementation of a GUI made with QT.
Extensibility is a key aspect of the project, as it is possible to add new types of media files and new metadata fields.
For the purpose of the project, there are 4 concrete classes:
- Poem 
- Book
both derived from the abstract class Literature, and
- AcademicArticle
- NewspaperArticle
both derived from the abstract class Article.