-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Find reports regarding theft
SELECT * FROM crime_scene_reports WHERE description LIKE "%theft%";
-- Find interviews regarding thief from the day of the theft
SELECT * FROM interviews WHERE year = 2021 AND month = 7 AND day = 28
AND transcript LIKE "%thief%";
-- Find atm transaction information for given time frame at Leggett street
SELECT * FROM atm_transactions 
WHERE year = 2021 AND month = 7 AND day = 28
AND atm_location = "Leggett Street";
-- Find account numbers for above. only withdrawals.
SELECT account_number FROM atm_transactions 
WHERE year = 2021 AND month = 7 AND day = 28
AND atm_location = "Leggett Street"
AND transaction_type = "withdraw";
-- Find suspect bank accounts
SELECT * FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions 
WHERE year = 2021 AND month = 7 AND day = 28
AND atm_location = "Leggett Street"
AND transaction_type = "withdraw");
-- Find bank suspect names
SELECT name FROM people WHERE id IN
(
SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions 
WHERE year = 2021 AND month = 7 AND day = 28
AND atm_location = "Leggett Street"
AND transaction_type = "withdraw")
);

-- Find camera information from bakery
SELECT * FROM bakery_security_logs 
WHERE year = 2021 AND month = 7 AND day = 28
AND hour = 10
AND minute > 15
AND minute < 25;
-- Find suspect car owner names
SELECT name FROM people WHERE license_plate IN
	(SELECT license_plate FROM bakery_security_logs 
	WHERE year = 2021 AND month = 7 AND day = 28
	AND hour = 10
	AND minute > 15
	AND minute < 25);

-- Find id of Fiftyville airport
SELECT id FROM airports WHERE city = "Fiftyville";
-- Find flights departing from Fiftyville the day after e.g. 29th
SELECT * FROM flights 
	WHERE year = 2021 AND month = 7 AND day = 29
	AND origin_airport_id IN
	(
		SELECT id FROM airports WHERE city = "Fiftyville"
	);
-- Find passports of said flights
SELECT passport_number FROM passengers WHERE flight_id IN
	(SELECT id FROM flights 
		WHERE year = 2021 AND month = 7 AND day = 29
		AND origin_airport_id IN
		(
			SELECT id FROM airports WHERE city = "Fiftyville"
		));

-- Find phone logs for that day with less than a minute duration
SELECT * FROM phone_calls 
		WHERE year = 2021 AND month = 7 AND day = 28
		AND duration < 60;
-- Find suspect callers
SELECT name FROM people WHERE phone_number IN
(
	SELECT caller FROM phone_calls 
			WHERE year = 2021 AND month = 7 AND day = 28
			AND duration < 60
);

-- Find suspects that match callers, car and bank suspects
SELECT name FROM people WHERE name IN
	(SELECT name FROM people WHERE phone_number IN
	(
		SELECT caller FROM phone_calls 
				WHERE year = 2021 AND month = 7 AND day = 28
				AND duration < 60
	))
AND name IN
(
SELECT name FROM people WHERE license_plate IN
	(SELECT license_plate FROM bakery_security_logs 
	WHERE year = 2021 AND month = 7 AND day = 28
	AND hour = 10
	AND minute > 15
	AND minute < 25)
)
AND name IN
(

SELECT name FROM people WHERE id IN
(
SELECT person_id FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions 
WHERE year = 2021 AND month = 7 AND day = 28
AND atm_location = "Leggett Street"
AND transaction_type = "withdraw")
)
);

-- Find phone call made by Diana or Bruce, thief candidates
SELECT name FROM people WHERE (name = "Diana" OR name = "Bruce")
AND phone_number IN
(
	SELECT caller FROM phone_calls 
			WHERE year = 2021 AND month = 7 AND day = 28
			AND duration < 60
);

-- Find phone call from thief candidate Diana
SELECT * FROM phone_calls 
	WHERE year = 2021 AND month = 7 AND day = 28
	AND caller IN
	(SELECT phone_number FROM people WHERE name = "Diana");

-- Find name of accomplice by seeing who thief called
SELECT name FROM people WHERE phone_number IN
(SELECT receiver FROM phone_calls 
	WHERE year = 2021 AND month = 7 AND day = 28
	AND duration < 60
	AND caller IN
	(SELECT phone_number FROM people WHERE name = "Bruce"));

-- Find flight where Diana was on passenger list and date was 29th and leaving Fiftyville.
SELECT * from FLIGHTS WHERE id IN
(SELECT flight_id FROM passengers WHERE year = 2021 AND month = 7 AND day = 29
	AND passport_number IN(
		SELECT passport_number FROM people WHERE name = "Bruce"
));

-- Find name of airport city for above flight
SELECT city FROM airports WHERE id IN
(SELECT destination_airport_id from FLIGHTS WHERE id IN
(SELECT flight_id FROM passengers WHERE year = 2021 AND month = 7 AND day = 29
	AND passport_number IN(
		SELECT passport_number FROM people WHERE name = "Bruce"
)));
