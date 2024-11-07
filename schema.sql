CREATE TABLE IF NOT EXISTS blocks(
	id                INTEGER PRIMARY KEY AUTOINCREMENT,
	creation_time     INTEGER,
	is_document       INTEGER,
	modification_time INTEGER,
  position          INTEGER,
  parent_id         INTEGER,
  expanded          INTEGER,
	content           TEXT
);
