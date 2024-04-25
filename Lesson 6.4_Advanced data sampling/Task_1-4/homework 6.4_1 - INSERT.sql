INSERT INTO artists (artist_name)
VALUES
	('Ivan Dorn'),
	('Petr Lom'),
	('Artem'),
	('Irina Crown'),
	('Alena'),
	('Victor');
	
INSERT INTO genres (genre)
VALUES
	('Pop'),
	('Rock'),
	('Jazz'),
	('Rap');
	
INSERT INTO albums (album_name, a_year)
VALUES
	('Meteora', 2019),
	('Happy time', 2020),
	('Forever alone', 2017),
	('Aero', 2018),
	('Hello', 2019);
	
INSERT  INTO tracks (id_album, track_name, duration)
VALUES
	(1, 'Numb', '00:03:45'),
	(1, 'From the inside', '00:03:21'),
	(2, 'My friend', '00:02:49'),
	(2, 'GO', '00:03:15'),
	(3, 'Lost', '00:03:58'),
	(3, 'Past time', '00:03:42'),
	(3, 'Remember', '00:03:02'),
	(3, 'Мой декабрь', '00:03:15'),
	(4, 'Fly with me', '00:03:35'),
	(4, 'Sky', '00:04:11'),
	(4, 'Arrived', '00:04:03'),
	(5, 'Hey!', '00:02:48'),
	(5, 'Welcome', '00:03:12'),
	(5, 'Thank you', '00:03:43');
	
INSERT INTO compilation (compilation_name, c_year)
VALUES
	('Dreamer', 2019),
	('Moonlight', 2017),
	('Active', 2020),
	('Good friend', 2019);
	
INSERT INTO artistalbums 
VALUES
	(1, 2),	(1, 3),
	(2, 3),
	(3, 5),	(3, 4),
	(4, 3),
	(5, 4),
	(6, 1);
	
INSERT INTO artistgenres 
VALUES
	(1, 1),
	(2, 1),
	(3, 2),	(3, 4),
	(4, 3),
	(5, 2),
	(6, 1),	(6, 2);
	
INSERT INTO compilationtracks 
VALUES
	(1, 6),	(1, 7),	(1, 9),
	(2, 5),	(2, 7),	(2, 8),	(2, 14),
	(3, 1),	(3, 2),	(3, 4),	(3, 12), (3, 13),
	(4, 3), (4, 7), (4, 8), (4, 11);
	