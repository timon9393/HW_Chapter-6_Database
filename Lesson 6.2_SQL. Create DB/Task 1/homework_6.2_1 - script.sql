CREATE TABLE IF NOT EXISTS Genres (
	id SERIAL PRIMARY KEY,
	genre VARCHAR(30) NOT NULL
);

CREATE TABLE IF NOT EXISTS Artists (
	id SERIAL PRIMARY KEY,
	artist_name VARCHAR(30) NOT NULL
);

CREATE TABLE IF NOT EXISTS Albums (
	id SERIAL PRIMARY KEY,
	album_name VARCHAR(30) NOT NULL,
	a_year INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS Tracks (
	id SERIAL PRIMARY KEY,
	id_album INTEGER,
	FOREIGN KEY (id_album) REFERENCES Albums(id),
	track_name VARCHAR(30) NOT NULL,
	duration INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS Compilation (
	id SERIAL PRIMARY KEY,
	compilation_name VARCHAR(30) NOT NULL,
	c_year INTEGER NOT NULL
);

CREATE TABLE IF NOT EXISTS ArtistGenres (
	id_artist INTEGER,
	FOREIGN KEY (id_artist) REFERENCES Artists(id),
	id_genre INTEGER,
	FOREIGN KEY (id_genre) REFERENCES Genres(id),
	PRIMARY KEY (id_artist, id_genre)
);

CREATE TABLE IF NOT EXISTS ArtistAlbums (
	id_artist INTEGER,
	FOREIGN KEY (id_artist) REFERENCES Artists(id),
	id_album INTEGER,
	FOREIGN KEY (id_album) REFERENCES Albums(id),
	PRIMARY KEY (id_artist, id_album)
);

CREATE TABLE IF NOT EXISTS CompilationTracks (
	id_compilation INTEGER,
	id_track INTEGER,
	PRIMARY KEY (id_compilation, id_track),
	FOREIGN KEY (id_compilation) REFERENCES Compilation(id),
	FOREIGN KEY (id_track) REFERENCES Tracks(id)
);