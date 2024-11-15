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

CREATE VIRTUAL TABLE IF NOT EXISTS blocks_fts5 using fts5(id, content);

CREATE TRIGGER IF NOT EXISTS blocks_insert_trigger AFTER INSERT ON blocks
FOR EACH ROW BEGIN INSERT INTO blocks_fts5 (id, content) VALUES (NEW.id, NEW.content); END;

CREATE TRIGGER IF NOT EXISTS blocks_update_trigger AFTER UPDATE ON blocks 
BEGIN UPDATE blocks_fts5 SET id = NEW.id, content = NEW.content WHERE id = NEW.id; END;

CREATE TRIGGER IF NOT EXISTS blocks_delete_trigger AFTER DELETE ON blocks
BEGIN DELETE FROM blocks_fts5 WHERE id = OLD.id; END;
