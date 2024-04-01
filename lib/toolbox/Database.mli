(** Utilities for working with the PostgreSQL database. *)

type database_options = {
  credentials : (string * string) option;
  host : string;
  port : string;
}
(** Options used to construct the PostgreSQL connection URI. *)

val get_options_from_env : unit -> database_options
(** [get_options_from_env ()] parses [database_options] from the following
    environment variables:
    
    - [credentials] - [SABIHIN_DB_USERNAME], [SABIHIN_DB_PASSWORD]
    - [host] - [SABIHIN_DB_HOST], default [localhost]
    - [port] - [SABIHIN_DB_PORT], default [5432]
*)

val make_url : options:database_options -> string -> string
(** [make_url ~options database] creates a PostgreSQL connection URI with the
    provided [~options] and [database] name. *)
