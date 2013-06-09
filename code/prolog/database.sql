
drop table if exists modules;

create table if not exists modules (
	id int not null primary key auto_increment,
	module varchar (128) not null unique
);

drop table if exists atoms;

create table if not exists atoms (
	id int not null primary key auto_increment,
	module_id int not null,
	atom varchar (128) not null
);

drop table if exists clauses;

create table if not exists clauses (
	id int not null primary key auto_increment,
	module_id int not null,
	atom_id int not null,
	ordering int not null,
	parameters_id int not null,
	body_id int not null
);

drop table if exists text_clauses;

create table if not exists text_clauses (
	id int not null primary key auto_increment,
	module_id int not null,
	atom_id int not null,
	ordering int not null,
	text text not null
);

drop table if exists elements;

create table if not exists elements (
	id int not null primary key auto_increment,
	module_id int not null,
	atom_id int not null,
	clause_id int not null,
	type int not null,
	reference int,
	reference_right int,
	value float
);

drop table if exists texts;

create table if not exists texts (
	id int not null primary key auto_increment,
	module_id int not null,
	atom_id int not null,
	clause_id int not null,
	text text not null
);

/*

insert into modules (module) values ('castle'), ('island');

insert into atoms (module_id, atom) values
	(1, 'princess'),
	(1, 'dragon'),
	(1, 'tower'),
	(1, 'bridge'),
	(1, 'wall');

insert into atoms (module_id, atom) values
	(2, 'beach'),
	(2, 'palm'),
	(2, 'bottle'),
	(2, 'jungle');
*/
