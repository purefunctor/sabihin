val connect_pool :
  unit ->
  (Caqti_lwt.connection, [> Caqti_error.connect ]) Caqti_lwt.Pool.t Lwt.t

val make_test_case :
  (Caqti_lwt.connection -> ('a, Caqti_error.t) Lwt_result.t) ->
  (Caqti_lwt.connection, Caqti_error.t) Caqti_lwt.Pool.t ->
  Lwt_switch.t ->
  unit ->
  'a Lwt.t
