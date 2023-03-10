SELECT title FROM movies 
WHERE movies.id IN
	(SELECT movie_id FROM stars JOIN people ON people.id = stars.person_id WHERE people.name = "Helena Bonham Carter")
	AND movies.id IN
	(SELECT movie_id FROM stars JOIN people ON people.id = stars.person_id WHERE people.name = "Johnny Depp");
