module AppPage = {
  [@react.component]
  let make = (~path) => {
    <Page> <App serverUrl={path, hash: "", search: ""} /> </Page>;
  };
};

let renderPage = path => ReactDOM.renderToStaticMarkup(<AppPage path />);

Dream.run @@
Dream.logger @@
Dream.router([
  Dream.get("assets/**", Dream.static("assets")),
  Dream.get("static/**", Dream.static("dist")),
  Dream.get("/", _ => {[] |> renderPage |> Dream.html}),
]);
