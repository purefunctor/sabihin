(** Test-specific toolbox. *)

module Database : sig
  include module type of Toolbox.Database

  (** Common database queries for tests. *)
  module Q : sig
    val create_database :
      Caqti_lwt.connection ->
      string ->
      (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
    (** [create_database db name] creates a database named [name].*)

    val drop_database :
      Caqti_lwt.connection ->
      string ->
      (unit, [> Caqti_error.call_or_retrieve ]) result Lwt.t
    (**  [drop_database db name] drops a database named [name]. *)

    val initialize_database :
      Caqti_lwt.connection ->
      string ->
      (unit, [> Caqti_error.call_or_retrieve ]) Lwt_result.t
    (** [initialize_database db name] initializes a database named [name]. 
        
        Initialization is the combination of [drop_database] and [create_database]. 
        In particular, this is useful for setting up and tearing down ephemeral 
        databases used by tests. *)
  end
end
