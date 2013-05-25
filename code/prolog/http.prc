
program http #machine := "prolog.http"

	[
		webserver FULL_HEADER_TEXT param HTTP_URI HTTP_PROTOCOL HTTP_HEADER
		GET POST PUT PATCH DELETE COPY HEAD OPTIONS LINK UNLINK PURGE
		GETter
	]

#machine webserver := "webserver"

[[GET *res *content] / [*res "<a href=\"/\" />" *content "</a>"]]
[[GET *res : *command] / [*res "<a href=\""] / [GETter *res : *command]]
[[GETter *res *p1 *p2 : *tail] / [*res "/" *p1] / [GETter *res *p2 : *tail]]
[[GETter *res *content] [*res "\" />" *content "</a>"]]
[[GETter *res]]

protect [GET GETter]
private [GETter]

end .

