CREATE TABLE IF NOT EXISTS Seed (
    seed_id INTEGER NOT NULL,
    seed_plant_name TEXT,
    seed_variety_name TEXT,
    seed_binomial_name TEXT,
    seed_description TEXT,
    CONSTRAINT Seed_PK PRIMARY KEY (seed_id)
);

CREATE TABLE IF NOT EXISTS Date (
    date_id INTEGER NOT NULL,
    date_name TEXT NOT NULL,
    date_months INTEGER,
    CONSTRAINT Date_PK PRIMARY KEY (date_id)
);

CREATE TABLE IF NOT EXISTS Image (
    image_id INTEGER NOT NULL,
    image_path TEXT NOT NULL,
    image_name TEXT,
    image_description TEXT,
    CONSTRAINT Image_PK PRIMARY KEY (image_id)
);

CREATE TABLE IF NOT EXISTS Category (
    category_id INTEGER NOT NULL,
    category_name TEXT NOT NULL,
    CONSTRAINT Category_PK PRIMARY KEY (category_id)
);

CREATE TABLE IF NOT EXISTS ClassifiedBy (
    seed_id INTEGER NOT NULL,
    category_id INTEGER NOT NULL,
    CONSTRAINT ClassifiedBy_Seed_FK FOREIGN KEY (seed_id) REFERENCES Seed(seed_id),
    CONSTRAINT ClassifiedBy_Category_FK FOREIGN KEY (category_id) REFERENCES Category(category_id)
);

CREATE TABLE IF NOT EXISTS IllustratedBy (
    seed_id INTEGER NOT NULL,
    image_id INTEGER NOT NULL,
    CONSTRAINT IllustratedBy_Seed_FK FOREIGN KEY (seed_id) REFERENCES Seed(seed_id),
    CONSTRAINT IllustratedBy_Image_FK FOREIGN KEY (image_id) REFERENCES Image(image_id)
);

CREATE TABLE IF NOT EXISTS DateAnnotedBy (
    seed_id INTEGER NOT NULL,
    date_id INTEGER NOT NULL,
    CONSTRAINT DateAnnotedBy_Seed_FK FOREIGN KEY (seed_id) REFERENCES Seed(seed_id),
    CONSTRAINT DateAnnotedBy_Date_FK FOREIGN KEY (date_id) REFERENCES Date(date_id)
);
