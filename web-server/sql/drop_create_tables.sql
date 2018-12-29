DROP TABLE IF EXISTS User_Login;
/* Order matters */

CREATE TABLE User_Login (
       User_ID INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
       Username varchar(255) NOT NULL UNIQUE,
       Email varchar(255) NOT NULL UNIQUE,
       Password varchar(255) NOT NULL
);
