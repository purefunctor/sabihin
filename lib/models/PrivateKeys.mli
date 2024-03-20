(** Represents a {!User}'s private keys. *)

type t = {
  user_id : int32;  (** References a {!User.t.id}. *)
  client_random_value : Bytes.t;
      (** A random value generated by a CSPRNG in the client.

          Typically 16 bytes in length, this is used as part of the salt
          for deriving the bits in the "[DerivedKey]". *)
  encrypted_master_key : Bytes.t;
      (** The symmetric key used to encrypt the {!encrypted_protection_key}
          and {!encrypted_verification_key}. *)
  master_key_iv : Bytes.t;
      (** The [iv] used to encrypt the {!encrypted_master_key}. *)
  encrypted_protection_key : Bytes.t;
      (** The private key used to decrypt the "[EphemeralKey]" that's passed
          alongside a message. *)
  protection_key_iv : Bytes.t;
      (** The [iv] used to encrypt the {!encrypted_protection_key}.  *)
  encrypted_verification_key : Bytes.t;
      (** The private key used to produce the signature that's passed alongside
          a message. *)
  verification_key_iv : Bytes.t;
      (** The [iv] used to encrypt the {!encrypted_verification_key}.  *)
}

val create_table :
  (module Rapper_helper.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [create_table db] creates the [private_keys] table in the [db] provided it 
    does not exist. *)

val get_by_user_id :
  user_id:int32 ->
  (module Rapper_helper.CONNECTION) ->
  (t, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [get_by_owner ~owner db] obtains a {!PrivateKeys.t} using an [~owner]. *)

val insert :
  user_id:int32 ->
  client_random_value:bytes ->
  encrypted_master_key:bytes ->
  master_key_iv:bytes ->
  encrypted_protection_key:bytes ->
  protection_key_iv:bytes ->
  encrypted_verification_key:bytes ->
  verification_key_iv:bytes ->
  (module Rapper_helper.CONNECTION) ->
  (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
(** [insert ... db] inserts a new row in the [private_keys] table. *)