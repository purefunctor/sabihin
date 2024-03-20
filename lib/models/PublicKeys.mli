(** Represents a {!User}'s public keys. *)

type t = {
  user_id : int32;  (** References a {!User.t.id}. *)
  protection_key : bytes;
      (** The public key used to encrypt the "[EphemeralKey]" that's passed
          alongside a message. *)
  verification_key : bytes;
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
  (t, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [get_by_owner ~owner db] obtains a {!PrivateKeys.t} using an [~owner]. *)

val insert :
  user_id:int32 ->
  protection_key:bytes ->
  verification_key:bytes ->
  (module Rapper_helper.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [insert ... db] inserts a new row in the [private_keys] table. *)