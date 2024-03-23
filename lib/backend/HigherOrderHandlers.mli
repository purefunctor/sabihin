(** Higher Order Handlers.
    
The {{: https://aantron.github.io/dream/ } dream} library implements handlers as
functions. Consequently, this allows us to write higher-order functions which
can be used to abstract over shared code that's more fit for handlers than
middleware.
*)

(** {1 with{i _x_ }body } 
  
with{i _x_}body handlers usually abstract over:
- parsing the request body into a more convenient type;
- asserting that the request expects a specific [Content-Type]. *)

val with_json_body :
  Dream.request ->
  (string -> 'json option) ->
  ('json -> Dream.response Lwt.t) ->
  Dream.response Lwt.t
(** [with_json_body request parser handler] parses the [request] body with a
    JSON [parser] after which it's passed to the [handler]. This also asserts
    that the request's [Content-Type] header is set to [application/json].
    
    Responds with [400 Bad Request] if an invalid [Content-Type] is given, or 
    if the given [parser] fails. *)
