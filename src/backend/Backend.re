module AppPage = {
  [@react.component]
  let make = (~path) => {
    <Page> <App serverUrl={path, hash: "", search: ""} /> </Page>;
  };
};

let renderPage = path => ReactDOM.renderToStaticMarkup(<AppPage path />);

let error_template = (_, _, suggested_response) => {
  let status = Dream.status(suggested_response);
  let code = Dream.status_to_int(status);
  let reason = Dream.status_to_string(status);

  switch (code) {
  | 404 =>
    Dream.set_header(suggested_response, "Content-Type", Dream.text_html);
    Dream.set_body(suggested_response, ["not-found"] |> renderPage);
    Lwt.return(suggested_response);
  | _ =>
    Dream.set_header(
      suggested_response,
      "Content-Type",
      Dream.application_json,
    );
    Dream.set_body(
      suggested_response,
      Printf.sprintf({|{"code": %i, "reason": "%s"}|}, code, reason),
    );
    Lwt.return(suggested_response);
  };
};

Dream.run(
  ~error_handler=Dream.error_template(error_template),
  ~interface="0.0.0.0",
) @@
Dream.logger @@
Dream.router([
  Dream.get("assets/**", Dream.static("assets")),
  Dream.get("static/**", Dream.static("dist")),
  Dream.get("/", _ => {[] |> renderPage |> Dream.html}),
]);
