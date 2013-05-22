
program http #machine := "prolog.http"

	[
		webserver FULL_HEADER_TEXT param METHOD HTTP_URI HTTP_PROTOCOL HTTP_HEADER
		GET POST PUT PATCH DELETE COPY HEAD OPTIONS LINK UNLINK PURGE
	]

#machine webserver := "webserver"

end .

