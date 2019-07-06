--usage: sudo mysql -uroot < myselect.sql | source myselect.sql
drop database exercise; 
create database exercise; 
use exercise;
--chinese
create table chinese (name char, score int);
insert into chinese values('a', 91);
insert into chinese values('b', 11);
insert into chinese values('c', 21);
insert into chinese values('d', 31);
insert into chinese values('e', 41);
insert into chinese values('f', 51);
--math table
create table math (name char, score int);
insert into math values('a',92);
insert into math values('b',12);
insert into math values('c',22);
insert into math values('d',32);
insert into math values('e',42);
insert into math values('f',52);
