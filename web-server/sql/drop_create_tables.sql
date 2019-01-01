DROP TABLE IF EXISTS Conversations;

CREATE TABLE Conversations (
       Conversation_ID INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
       Time_Stamp varchar(255) NOT NULL UNIQUE,
       File_Name varchar(255) NOT NULL UNIQUE
);
