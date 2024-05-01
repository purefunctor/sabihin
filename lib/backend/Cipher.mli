(** Cipher secrets and operations.
   
    This module implements utilities for cipher operations and acts as a module
    for storing various secrets. Notably, this module implements the {!double_hmac}
    function, which is used for password storage, and the {!server_salt} function,
    which is used in the login process.

    Secrets are usually set through environment variables parsed by the backend
    executable, or arbitrarily by test code. It's highly recommended that these
    secrets are cryptographically-secure values.
*)

val set_cipher_secret : string -> unit
(** [set_cipher_secret value] sets the cipher secret, which is used as a key for
    operations such as {!double_hmac}. *)

val get_cipher_secret : unit -> Cstruct.t
(** [get_cipher_key ()] gets the current cipher secret, raising if it had not
    been set. *)

val set_server_random : string -> unit
(** [set_server_random value] sets the server random value, a string of {b 16
    characters} which is used to compute the salt sent to the client if a user
    does not exist in the database during the login process. *)

val get_server_random : unit -> Bytes.t
(** [get_cipher_key ()] gets the current server random value, raising if it had
    not been set. *)

val double_hmac : string -> string
(** [double_hmac value] calculates the HMAC of the HMAC of the [value] with the
    cipher secret as the key. *)

val server_salt : unit -> string
(** [server_salt ()] calculates the salt sent to the client if a user does not
    exist in the database during the login process. *)
