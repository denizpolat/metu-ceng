
CREATE OR REPLACE FUNCTION q1() RETURNS TRIGGER AS $update_review$
    BEGIN
        UPDATE Users u
        SET review_count = review_count + 1
        WHERE u.user_id = new.user_id;
        RETURN new;
    END;
    $update_review$ LANGUAGE plpgsql;

CREATE TRIGGER update_review
    AFTER INSERT ON Review
    EXECUTE PROCEDURE q1();








CREATE OR REPLACE FUNCTION q2() RETURNS TRIGGER AS $no_zeros_allowed$
    BEGIN
        IF (new.stars = 0) THEN
            DELETE FROM Review r
            WHERE r.user_id = new.user_id;
            DELETE FROM Tip t
            WHERE t.user_id = new.user_id;
            RETURN null;
        END IF;
        RETURN new;
    END;
    $no_zeros_allowed$ LANGUAGE plpgsql;


CREATE TRIGGER no_zeros_allowed
    BEFORE INSERT ON Review
    FOR EACH ROW
    EXECUTE PROCEDURE q2();








CREATE VIEW BusinessCount
AS (
SELECT b.business_id, b.business_name, count(*) AS review_count
FROM Business b INNER JOIN Review r ON b.business_id = r.business_id
GROUP BY b.business_id, b.business_name
   );
