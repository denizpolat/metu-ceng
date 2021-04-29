CREATE DATABASE ceng352p1;


CREATE TABLE Business (
	business_id VARCHAR PRIMARY KEY,
	business_name VARCHAR NOT NULL,
	address VARCHAR,
	state VARCHAR NOT NULL,
	is_open BOOLEAN NOT NULL,
	stars FLOAT NOT NULL
);


COPY Business FROM '/home/deniz/Downloads/metu-ceng/spring2021/ceng352/project1/yelp_academic_dataset/yelp_academic_dataset_business.csv' DELIMITER ',' CSV HEADER;

CREATE TABLE Users (
	user_id VARCHAR PRIMARY KEY,
	user_name VARCHAR NOT NULL,
	review_count INT NOT NULL,
	yelping_since DATE NOT NULL,
	useful INT NOT NULL,
	funny INT NOT NULL,
	cool INT NOT NULL,
	fans INT NOT NULL,
	average_stars FLOAT NOT NULL
);


COPY Users FROM '/home/deniz/Downloads/metu-ceng/spring2021/ceng352/project1/yelp_academic_dataset/yelp_academic_dataset_user.csv' DELIMITER ',' CSV HEADER;


CREATE TABLE Friend (
	user_id1 VARCHAR REFERENCES Users(user_id),
	user_id2 VARCHAR REFERENCES Users(user_id),
	PRIMARY KEY (user_id1, user_id2)
);


COPY Friend FROM '/home/deniz/Downloads/metu-ceng/spring2021/ceng352/project1/yelp_academic_dataset/yelp_academic_dataset_friend.csv' DELIMITER ',' CSV HEADER;


CREATE TABLE Review (
	review_id VARCHAR PRIMARY KEY,
	user_id VARCHAR REFERENCES Users(user_id),
	business_id VARCHAR REFERENCES Business(business_id),
	stars FLOAT NOT NULL,
	date DATE NOT NULL,
	useful INT NOT NULL,
	funny INT NOT NULL,
	cool INT NOT NULL
);


COPY Review FROM '/home/deniz/Downloads/metu-ceng/spring2021/ceng352/project1/yelp_academic_dataset/yelp_academic_dataset_reviewNoText.csv' DELIMITER ',' CSV HEADER;



CREATE TABLE Tip (
	tip_text VARCHAR NOT NULL,
	date DATE NOT NULL,
	compliment_count INT NOT NULL,
	business_id VARCHAR REFERENCES Business(business_id),
	user_id VARCHAR REFERENCES Users(user_id)
);


COPY Tip FROM '/home/deniz/Downloads/metu-ceng/spring2021/ceng352/project1/yelp_academic_dataset/yelp_academic_dataset_tip.csv' DELIMITER ',' CSV HEADER;

ALTER TABLE Tip ADD COLUMN tip_id SERIAL PRIMARY KEY;

/* what i have done below is to re-order columns as given in pdf, Tip(tip id,business id, user id, date, tip text, compliment count). If order is not important, please ignore :) */
ALTER TABLE Tip RENAME TO oldtable;
CREATE TABLE Tip (
    tip_id INT PRIMARY KEY,
	business_id VARCHAR REFERENCES Business(business_id),
	user_id VARCHAR REFERENCES Users(user_id),
	date DATE NOT NULL,
	tip_text VARCHAR NOT NULL,
	compliment_count INT NOT NULL
);
INSERT INTO Tip (tip_id, business_id, user_id, date, tip_text, compliment_count) SELECT tip_id, business_id, user_id, date, tip_text, compliment_count FROM oldtable;

drop table oldtable;


