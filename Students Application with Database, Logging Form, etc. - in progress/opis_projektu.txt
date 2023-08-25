DATA SECURITY
In the project I used secure password storage. I use the well-known technique of hashing with pepper and salt. The real password is hashed and stored together with the unique salt for each user in the database. Separately, in the configuration file I store a unique pepper for each user. The verification process compares the hash from the database with the hash obtained by hashing the input along with the salt read from the database and the pepper read from the configuration file.For added security, the configuration file should be passworded.

*Hashing with salt: Using a unique salt for each user ensures that passwords are not hashed identically even with an identical password. This makes it difficult for attackers to use so-called rainbow tables, because they would have to create separate tables for each salt.

*Pepper: Incorporating an additional random pepper as an additional hashing element adds an extra layer of security. Since the pepper is not stored in the database, in the event of a database breach, attackers won't have access to these values.

*Hashed Configuration File: Hashing the configuration file that stores the pepper can provide additional protection against unauthorized access to these data. This way, if someone gains access to the file, they won't be able to read its contents.


Of course, we should also provide:
*Key Security: Ensuring that the keys used for hashing, salting, and other sensitive data are adequately secured is crucial. Access to these keys should be restricted to authorized personnel only.

*Updates and Monitoring: Regularly updating and monitoring the system is essential to ensure security. This allows for the detection and response to potential threats.

LIBRARY DATABASE
Database is used to manage a self-made library system designed and run by students. The book storage takes place in the room for student integration. Each participant is obliged to log in to their account and make changes on the computer located there before borrowing or returning any book. They can also make reservations from home to make sure they find the books they want on site. In the virtual book catalog, books can be searched by their title, author, and category. A specific book can have multiple copies and multiple authors can be associated with a book. Different publishers can publish the same book.

The system allows users to reserve books that are currently unavailable (when all copies have been borrowed). Students are notified by email about the due date for returning borrowed books, as well as when a book from the whishlist becomes available. Students are identified through their email addresses.


Tables:
*Book(id,title)
*Author(id, name)
*Book_author(book_id, author_id)
*Category(id, name)
*Publisher(id,name)
*Book_copy(id, year_published, book_id, publisher_id)
*User(id,email,username,...)
*Borrowing(id, start, end, book_copy_id, user_id, is_returned)
*Reservation(id, start, end, book_copy_id, user_id)
*Waitlist(user_id, book_id)
*Notification(id, sent_date, type, user_id, book_copy_id)

Observations, assumptions, explanations:

- A single book may have more than one author and a single author may write more than one book. It is a many-to-many relationship, so we create additional table book-author.

-In our library, each book is assigned to a category. A book may belong to only one category.

-In library there can be multiple copies of the same book. A book may also be published by multiple publishers. 

-We can only book a book if it is currently available and there are no other reservations for it - i.e. there can only be one reservation for each copy of the book at a time.

-In table Borrowings, we need a way to mark whether or not the book has been returned (even past the due date beacause  the book return may be overdue).

-Reservation end is the timestamp when the reservation ends. This is either when the reservation expires or when the book has been borrowed. If you want to pick up a reservation you have to do it at least one day before its end date.

-When the book is borrowed by the user who has placed a reservation on it, a borrowing entry is created, and the reservation entry is deleted. If a user does not borrow the book in time, the reservation simply expires.
