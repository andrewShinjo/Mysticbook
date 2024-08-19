CREATE TABLE IF NOT EXISTS blocks(
	id                INTEGER PRIMARY KEY AUTOINCREMENT,
	creation_time     INTEGER NOT NULL,
	modification_time INTEGER NOT NULL,
	content           TEXT    NOT NULL,
	is_document       INTEGER NOT NULL CHECK (is_document IN (0,1))
);
