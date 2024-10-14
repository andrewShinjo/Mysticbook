CREATE TABLE IF NOT EXISTS blocks(
	id                INTEGER PRIMARY KEY AUTOINCREMENT,
	creation_time     INTEGER NOT NULL,
	is_document       INTEGER NOT NULL CHECK(is_document IN (0,1)),
	modification_time INTEGER NOT NULL,
  position          INTEGER NOT NULL,
  parent_id         INTEGER NOT NULL,
	content           TEXT    NOT NULL,
  FOREIGN KEY(parent_id) REFERENCES blocks(id),
  UNIQUE(parent_id, position)
);
