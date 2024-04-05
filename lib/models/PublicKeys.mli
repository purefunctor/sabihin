(** Represents a {!User}'s public keys. *)

type t = {
  user_id : int32;  (** References a {!User.t.id}. *)
  exported_protection_key : bytes;
      (** The public key used to encrypt the "[EphemeralKey]" that's passed
          alongside a message. *)
  exported_verification_key : bytes;
      (** The public key used to verify the signature that's passed alongside
          a message. *)
}

val create_table :
  (module Rapper_helper.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [create_table db] creates the [public_keys] table in the [db] provided it 
    does not exist. *)

val get_by_user_id :
  user_id:int32 ->
  (module Rapper_helper.CONNECTION) ->
  (t option, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [get_by_user_id ~user_id db] obtains a {!PublicKeys.t} using a [~user_id]. *)

val get_by_username :
  username:string ->
  (module Rapper_helper.CONNECTION) ->
  (t option, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [get_by_username ~username db] obtains a {!PublicKeys.t} using a [~username]. *)

val get_by_public_id :
  public_id:string ->
  (module Rapper_helper.CONNECTION) ->
  (t option, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [get_by_public_id ~public_id db] obtains a {!PublicKeys.t} using a [~public_id]. *)

val insert :
  user_id:int32 ->
  exported_protection_key:bytes ->
  exported_verification_key:bytes ->
  (module Rapper_helper.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [insert ... db] inserts a new row in the [public_keys] table. *)
