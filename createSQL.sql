create database chat;

CREATE TABLE user  (
  `id` int PRIMARY KEY NOT NULL auto_increment,
  `username` char(20) NOT NULL,
  `password` char(20) NOT NULL,
  `createTime` datetime(0) NOT NULL
);