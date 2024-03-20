(** Initialization and migration routines for the models. *)

val initialize :
  (module Rapper_helper.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) Lwt_result.t
(** [initialize db] creates the model tables in the database. *)
