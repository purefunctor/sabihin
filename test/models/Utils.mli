val perish : ('a, Caqti_error.t) result Lwt.t -> 'a Lwt.t

val connect_pool :
  unit ->
  (Caqti_lwt.connection, [> Caqti_error.connect ]) Caqti_lwt.Pool.t Lwt.t

val initialize : unit -> unit Lwt.t

val make_test_case :
  ?speed:Alcotest.speed_level ->
  string ->
  (Caqti_lwt.connection -> (unit, Caqti_error.t) Lwt_result.t) ->
  (Caqti_lwt.connection, Caqti_error.t) Caqti_lwt.Pool.t ->
  unit Alcotest_lwt.test_case
