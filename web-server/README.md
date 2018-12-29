# React-Setup

## Installation

1. Install Node.js and all dependencies.
```bash
    sudo apt install nodejs
    cd /path/to/React-Setup
    ./install.sh
```

2. Install MySQL dependency.
```bash
    sudo apt install mysql-server
    sudo mysql
    mysql> SELECT user,authentication_string,plugin,host FROM mysql.user;
    mysql> ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'password';
    mysql> FLUSH PRIVILEGES;
    mysql> exit
```

3. Set up database credentials.
```bash
    cp skeleton-database.js config.js
    nano config.js
```

4. Set up database as needed.
```bash
    mysql -u root -p
    mysql> create database react_setup;
    mysql> use react_setup;
    mysql> source ~/path/to/file.sql;
```

5. Run on localhost.
```bash
   npm start
```

6. Host the project on a server instead of running on localhost. To
host with a domain name, view my [namecheap tutorial](https://github.com/ivanmanan/Web-Development/blob/master/raspberrypi/hosting.md).
```bash
    cd /path/to/React-Setup/client/
    npm run build
    cd ..
    npm run server
```

7. Leave the project running on the server.
```bash
    npm install -g pm2
    pm2 start server.js
```
