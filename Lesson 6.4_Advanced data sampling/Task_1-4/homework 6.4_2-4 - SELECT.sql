-- Задание 2
-- Название и продолжительность самого длительного трека.
SELECT track_name, duration FROM tracks
WHERE 
duration = (SELECT MAX (duration) FROM tracks);

-- Название треков, продолжительность которых не менее 3,5 минут.
SELECT track_name, duration FROM tracks
WHERE 
duration >= '00:03:30';

-- Названия сборников, вышедших в период с 2018 по 2020 годы включительно.
SELECT compilation_name, c_year FROM compilation
WHERE
c_year BETWEEN 2018 AND 2020;

-- Исполнители, чьё имя состоит из одного слова.
SELECT artist_name FROM artists
WHERE 
artist_name NOT LIKE '% %';

-- Название треков, которые содержат слова «Мой» или «My».
SELECT track_name FROM tracks
WHERE 
track_name LIKE '%My%' OR track_name LIKE '%Мой%';


-- Задание 3
-- Количество исполнителей в каждом жанре
SELECT g.genre, COUNT(id_artist) number_of_artists FROM artistgenres a 
LEFT JOIN genres g ON a.id_genre = g.id 
GROUP BY g.genre
ORDER BY COUNT(id_artist) DESC;

-- Количество треков, вошедших в альбомы 2019–2020 годов
SELECT a.album_name, COUNT(id_album) number_of_tracks FROM tracks t 
LEFT JOIN albums a ON t.id_album = a.id 
WHERE a.a_year BETWEEN 2019 AND 2020
GROUP BY a.album_name 
ORDER BY COUNT(id_album) DESC; 

-- Средняя продолжительность треков по каждому альбому
SELECT a.album_name, AVG(duration) avg_duration FROM tracks t 
LEFT JOIN albums a ON t.id_album = a.id 
GROUP BY a.album_name
ORDER BY AVG(duration) DESC;

-- Все исполнители, которые не выпустили альбомы в 2020 году.
SELECT artist_name artists_without_albums_in_2020 FROM artists ar
WHERE ar.id NOT IN(
	SELECT id_artist FROM artistalbums aa
	LEFT JOIN albums al ON aa.id_album = al.id
	WHERE al.a_year = 2020
);

-- Названия сборников, в которых присутствует конкретный исполнитель (Alena)
SELECT compilation_name FROM compilation c 
LEFT JOIN compilationtracks ct ON c.id = ct.id_compilation 
LEFT JOIN tracks t ON ct.id_track = t.id 
LEFT JOIN albums al ON t.id_album = al.id
LEFT JOIN artistalbums aa ON al.id = aa.id_album 
LEFT JOIN artists a ON aa.id_artist = a.id 
WHERE artist_name LIKE 'Alena'
GROUP BY compilation_name;


-- Задание 4
-- Названия альбомов, в которых присутствуют исполнители более чем одного жанра
SELECT DISTINCT album_name FROM albums a 
LEFT JOIN artistalbums aa ON a.id = aa.id_album 
LEFT JOIN artists ar ON aa.id_artist = ar.id 
WHERE ar.id IN (
	SELECT id_artist FROM artistgenres ag
	GROUP BY id_artist
	HAVING COUNT(id_genre) > 1
);

-- Наименования треков, которые не входят в сборники.
SELECT track_name FROM tracks t 
LEFT JOIN compilationtracks c ON t.id = c.id_track 
WHERE c.id_track IS NULL;

-- Исполнитель или исполнители, написавшие самый короткий по продолжительности трек, — теоретически таких треков может быть несколько.
SELECT artist_name FROM artists a 
WHERE a.id IN(
	SELECT id_artist FROM artistalbums aa
	JOIN albums al ON aa.id_album = al.id
	JOIN tracks t ON al.id = t.id_album
	WHERE t.duration IN (
		SELECT MIN(duration) FROM tracks
	)
);

-- Названия альбомов, содержащих наименьшее количество треков.
SELECT album_name FROM albums al 
WHERE id IN (
	SELECT id_album FROM tracks
	GROUP BY id_album
	HAVING COUNT(id_album) = (
		SELECT count(id_album) FROM tracks
		GROUP BY id_album
		ORDER BY count(id_album)
		LIMIT 1
	)
);
