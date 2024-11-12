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

CREATE VIRTUAL TABLE IF NOT EXISTS blocks_fts5 using fts5(content);

CREATE TRIGGER IF NOT EXISTS blocks_insert_trigger
AFTER INSERT ON blocks
FOR EACH ROW
  BEGIN
    INSERT INTO blocks_fts5 (content) VALUES (new.content);
  END;
