

/* Question 1 */

SELECT DISTINCT u.user_id, u.user_name, (u.review_count - u.fans) AS difference
FROM Users u INNER JOIN Review r ON u.user_id = r.user_id
INNER JOIN Business b ON b.business_id = r.business_id
WHERE u.review_count > u.fans AND b.stars > 3.5
ORDER BY difference DESC, user_id DESC;



/* Question 2 */

SELECT u.user_name, b.business_name, t.date, t.compliment_count
FROM Users u INNER JOIN Tip t ON u.user_id = t.user_id
INNER JOIN Business b ON t.business_id = b.business_id
WHERE b.state = 'TX' AND b.is_open AND t.compliment_count > 2
ORDER BY t.compliment_count DESC, t.date DESC;



/* Question 3 */


SELECT u.user_name, count(user_id2) AS friend_count
FROM Friend f INNER JOIN Users u ON u.user_id = f.user_id1
GROUP BY u.user_id, u.user_name
ORDER BY friend_count DESC, u.user_name DESC
LIMIT 20;


/*
SELECT u.user_name, fr.count as friend_count
FROM Users u INNER JOIN (
    SELECT f.user_id1, count(user_id2) AS count
    FROM Friend f
    GROUP BY f.user_id1
    ) AS fr ON u.user_id = fr.user_id1
ORDER BY friend_count DESC, u.user_name DESC
LIMIT 20;
*/




/* Question 4 */

SELECT u.user_name, u.average_stars, u.yelping_since
FROM Users u
WHERE u.user_id IN (
    SELECT DISTINCT u2.user_id
    FROM Users u2 INNER JOIN Review r ON u2.user_id = r.user_id
    INNER JOIN Business b ON b.business_id = r.business_id
    WHERE b.stars > r.stars
    )
ORDER BY u.average_stars DESC, u.yelping_since DESC;



/* Question 5 */

SELECT b.business_name, b.state, b.stars
FROM Business b
WHERE b.business_id IN (
    SELECT t.business_id
    FROM Tip t
    WHERE t.tip_text LIKE 'good' AND t.date >= '2020-01-01'::date AND t.date <= '2020-12-31'::date
    GROUP BY t.business_id
    ORDER BY count(*) DESC
    ) AND b.is_open
ORDER BY b.stars DESC, b.business_name DESC;



/* Question 6 */

SELECT u1.user_name, u1.yelping_since, u1.average_stars
FROM Users u1 INNER JOIN (
    SELECT f.user_id1, min(u2.average_stars) AS min
    FROM Friend f INNER JOIN Users u2 ON u2.user_id = f.user_id2
    GROUP BY f.user_id1
    ) AS friends ON u1.user_id = friends.user_id1
WHERE u1.average_stars < min
ORDER BY u1.average_stars DESC, u1.yelping_since DESC;




/* Question 7 */

SELECT b.state, AVG(b.stars) AS average_stars
FROM Business b
GROUP BY b.state
ORDER BY average_stars DESC
LIMIT 10;



/* Question 8 */

SELECT table2.year2 AS year, table2.average AS average_compliment_count
FROM (
    SELECT EXTRACT(YEAR FROM t1.date) as year1, count(t1) AS c1
    FROM Tip t1
    WHERE t1.compliment_count != 0
    GROUP BY year1) AS table1 INNER JOIN (
        SELECT EXTRACT(YEAR FROM t2.date) as year2, count(t2) AS c2, AVG(t2.compliment_count) AS average
        FROM Tip t2
        GROUP BY year2
    ) AS table2 ON table1.year1 = table2.year2
WHERE table1.c1::decimal / table2.c2::decimal > 0.01
ORDER BY year ASC;




/* Question 9 */

SELECT u.user_name
FROM Users u INNER JOIN (
    (SELECT DISTINCT r.user_id
    FROM Users u INNER JOIN Review r on u.user_id = r.user_id)
    EXCEPT
    (SELECT DISTINCT r2.user_id
    FROM Review r2 INNER JOIN Business b ON r2.business_id = b.business_id
    WHERE b.stars <= 3.5)
    ) AS ids ON u.user_id = ids.user_id
ORDER BY u.user_name ASC;



/* Question 10 */

SELECT b.business_name as name, year_star.year, year_star.avg as average_stars
FROM (
    SELECT r.business_id
    FROM Review r
    GROUP BY r.business_id
    HAVING count(*) > 1000
         ) AS bids INNER JOIN (
             SELECT r2.business_id, EXTRACT(YEAR FROM r2.date) as year, AVG(r2.stars) as avg
             FROM Review r2
             GROUP BY r2.business_id, year
    ) AS year_star ON bids.business_id = year_star.business_id
    INNER JOIN Business b ON b.business_id = year_star.business_id
WHERE year_star.avg > 3
ORDER BY year ASC, name ASC;




/* Question 11 */

SELECT users.user_name, counts.u AS useful, counts.c AS cool, (counts.u - counts.c) AS difference
FROM (
    SELECT r.user_id, SUM(r.useful) AS u, SUM(r.cool) AS c
    FROM Review r
    GROUP BY r.user_id
         ) AS counts INNER JOIN Users users ON counts.user_id = users.user_id
WHERE counts.u > counts.c
ORDER BY difference DESC, users.user_name DESC;




/* Question 12 */

SELECT r1.business_id, f.user_id1 AS user_id_1, f.user_id2 AS user_id_2, r1.stars
    FROM (
        (SELECT * FROM Friend f1)
        EXCEPT
        (SELECT f1.user_id1, f1.user_id2 FROM Friend f1 INNER JOIN Friend f2 ON f1.user_id1 = f2.user_id2 AND f1.user_id2 = f2.user_id1 WHERE f1.user_id1 < f1.user_id2)
             ) as f INNER JOIN (
                 SELECT DISTINCT ON (r11.user_id, r11.business_id, r11.stars) r11.user_id, r11.business_id, r11.stars
                 FROM Review r11
        ) AS r1 ON r1.user_id = f.user_id1 INNER JOIN (
            SELECT DISTINCT ON (r22.user_id, r22.business_id, r22.stars) r22.user_id, r22.business_id, r22.stars
            FROM Review r22
        ) AS r2 ON r2.user_id = f.user_id2 AND r1.business_id = r2.business_id AND r1.stars = r2.stars
ORDER BY r1.business_id DESC, r1.stars DESC;




/* Question 13 */

SELECT stars, state, count(*) as count
FROM Business b
WHERE is_open
GROUP BY CUBE(stars, state);




/* Question 14 */

SELECT itable.user_id, itable.review_count, itable.fans, itable.rank
FROM (
    SELECT u.user_id, u.review_count, u.fans,
        RANK() OVER (PARTITION BY u.fans ORDER BY u.review_count DESC) AS rank
    FROM Users u
    WHERE u.fans >= 50 AND u.fans <= 60
) AS itable
WHERE itable.rank <= 3;



