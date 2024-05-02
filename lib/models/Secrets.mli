(** Represents a {!User}'s secrets. *)

type t = {
  user_id : int32;  (** References a {!User.t.id}. *)
  encrypted_master_key : string;
      (** The symmetric key used to encrypt the {!encrypted_protection_key}
          and {!encrypted_verification_key}. *)
  encrypted_protection_key : string;
      (** The private key used to decrypt the "[EphemeralKey]" that's passed
          alongside a message. *)
  exported_protection_key : string;
      (** The public key used to encrypt the "[EphemeralKey]" that's passed
          alongside a message. *)
  encrypted_verification_key : string;
      (** The private key used to produce the signature that's passed alongside
          a message. *)
  exported_verification_key : string;
      (** The public key used to verify the signature that's passed alongside
          a message. *)
}

val create_table :
  (module Rapper_helper.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [create_table db] creates the [secrets] table in the [db] provided it does
    not exist. *)

val get_by_user_id :
  user_id:int32 ->
  (module Rapper_helper.CONNECTION) ->
  (t option, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [get_by_user_id ~user_id db] obtains a {!Secrets.t} using an [~user_id]. *)

val get_by_username :
  username:string ->
  (module Rapper_helper.CONNECTION) ->
  (t option, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [get_by_username ~username db] obtains a {!Secrets.t} using a [~username]. *)

val insert :
  user_id:int32 ->
  encrypted_master_key:string ->
  encrypted_protection_key:string ->
  exported_protection_key:string ->
  encrypted_verification_key:string ->
  exported_verification_key:string ->
  (module Rapper_helper.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [insert ... db] inserts a new row in the [secrets] table. *)
