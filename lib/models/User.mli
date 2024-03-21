(** Represents the users of the application. *)

type t = {
  id : int32;
  username : string;
  auth_token : string;
      (** A SHA-512 hash representing the authentication
          token derived from the user's password. *)
}

val create_table :
  (module Rapper_helper.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [create_table db] creates the [users] table in the [db] provided it does 
    not exist. *)

val get_by_id :
  id:int32 ->
  (module Rapper_helper.CONNECTION) ->
  (t option, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [get_by_id ~id db] obtains a {!User.t} using an [~id]. *)

val get_by_username :
  username:string ->
  (module Rapper_helper.CONNECTION) ->
  (t option, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [get_by_username ~username db] obtains a {!User.t} using a [~username]. *)

val insert :
  username:string ->
  auth_token:string ->
  (module Rapper_helper.CONNECTION) ->
  (int32, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [insert ~username ~auth_token db] insert a new row in the [users] table,
    returning the user's [id]. *)
