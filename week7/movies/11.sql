SELECT title FROM movies JOIN ratings ON movies.id = ratings.movie_id 
WHERE movies.id IN
	(SELECT movie_id FROM stars JOIN people ON people.id = stars.person_id WHERE people.name = "Chadwick Boseman")
ORDER BY rating DESC
LIMIT 5;
