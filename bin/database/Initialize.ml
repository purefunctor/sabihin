open Cmdliner

let perish action =
  action
  |> Lwt_result.map_error (fun e -> Caqti_error.Exn e)
  |> Lwt_result.get_exn

let initialize log_level username password host port db =
  Logs.set_level log_level;

  let open Test_toolbox.Database in
  let credentials =
    let ( let* ) = Option.bind in
    let* username = username in
    let* password = password in
    Some (username, password)
  in

  let options = { credentials; host; port } in

  let default_url = make_url ~options "" in
  Logging.info (fun log -> log "using default_url: %s" default_url);

  let db_url = make_url ~options db in
  Logging.info (fun log -> log "using db_url: %s" db_url);

  let inner =
    let open Lwt_result.Infix in
    Caqti_lwt.with_connection (Uri.of_string default_url) (fun connection ->
        Q.create_database connection db)
    >>= fun _ ->
    Caqti_lwt.with_connection (Uri.of_string db_url) (fun connection ->
        Models.Initialize.initialize connection)
    >>= fun _ ->
    Lwt_result.ok
    @@ Logs_lwt.info ~src:Logging.src (fun log ->
           log "finished initializing: %s" db)
  in

  Lwt_main.run @@ perish inner

let username =
  let doc = "The username to use for PostgreSQL." in
  let docv = "USERNAME" in
  let env = Cmd.Env.info "SABIHIN_DB_USERNAME" in
  Arg.(value & opt (some string) None & info [ "username" ] ~env ~doc ~docv)

let password =
  let doc = "The password to use for PostgreSQL." in
  let docv = "PASSWORD" in
  let env = Cmd.Env.info "SABIHIN_DB_PASSWORD" in
  Arg.(value & opt (some string) None & info [ "password" ] ~env ~doc ~docv)

let host =
  let doc = "The host to use for PostgreSQL." in
  let docv = "HOST" in
  let env = Cmd.Env.info "SABIHIN_DB_HOST" in
  Arg.(value & opt string "localhost" & info [ "host" ] ~env ~doc ~docv)

let port =
  let doc = "The port to use for PostgreSQL." in
  let docv = "PORT" in
  let env = Cmd.Env.info "SABIHIN_DB_PORT" in
  Arg.(value & opt string "5432" & info [ "port" ] ~env ~doc ~docv)

let db =
  let doc = "The name of the database to initialize." in
  let docv = "DATABASE_NAME" in
  Arg.(value & pos 0 string "sabihin" & info [] ~doc ~docv)

let initialize_t =
  Term.(
    const initialize $ Logs_cli.level () $ username $ password $ host $ port
    $ db)

let cmd =
  let info =
    let doc = "Initialize a database." in
    Cmd.info "initialize" ~doc
  in
  Cmd.v info initialize_t
