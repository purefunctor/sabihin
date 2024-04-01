module Database = struct
  include Toolbox.Database

  module Q = struct
    open Caqti_request.Infix
    open Caqti_type.Std

    let create_database ((module Db) : Caqti_lwt.connection) name =
      let statement = Printf.sprintf "CREATE DATABASE %s" name in
      let query = (unit ->. unit) ~oneshot:true statement in
      Db.exec query ()

    let drop_database ((module Db) : Caqti_lwt.connection) name =
      let statement = Printf.sprintf "DROP DATABASE IF EXISTS %s" name in
      let query = (unit ->. unit) ~oneshot:true statement in
      Db.exec query ()

    let initialize_database db name =
      let open Lwt_result.Infix in
      drop_database db name >>= fun _ -> create_database db name
  end
end
