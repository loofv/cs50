SELECT DISTINCT name FROM directors JOIN ratings ON directors.movie_id = ratings.movie_id
JOIN people ON person_id = id
WHERE rating >= 9.0;
