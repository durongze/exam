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
insert into chinese values('l', 71);
insert into chinese values('m', 81);
--math table
create table math (name char, score int);
insert into math values('a',92);
insert into math values('b',12);
insert into math values('c',22);
insert into math values('d',32);
insert into math values('e',42);
insert into math values('f',52);
insert into math values('u',72);
insert into math values('v',82);
--left join
select c.name,c.score,m.score from chinese as c left join math as m on c.name=m.name;
--right join
select m.name,c.score,m.score from chinese as c right join math as m on c.name=m.name;
--inner join
select c.name,c.score,m.score from chinese as c inner join math as m on c.name=m.name;
--outer join
select c.name,c.score,m.score from chinese as c left join math as m on c.name=m.name union select m.name,c.score,m.score from chinese as c right join math as m on c.name=m.name;
--score
create table score (name varchar(20), course varchar(20), score int);
insert into score values("zhangsan", "chinese", 81);
insert into score values("zhangsan", "english", 72);
insert into score values("lisi", "chinese", 91);
insert into score values("lisi", "english", 82);
insert into score values("wangwu", "chinese", 71);
insert into score values("wangwu", "english", 62);
insert into score values("zhaoliu", "chinese", 61);
insert into score values("zhaoliu", "english", 52);
--找所有分数不小于80的学生名 | 注意括号不能少,且括号内不能为空值
select s.name from score as s where s.name not in (select name from score where score < 80);
