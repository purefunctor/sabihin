module Page = {
  [@react.component]
  let make = (~children) => {
    <html lang="en">
      <head>
        <meta charSet="UTF-8" />
        <meta
          name="viewport"
          content="width=device-width, initial-scale=1.0"
        />
        <title> {React.string("Sabihin")} </title>
        <script type_="module" src="static/index.js" />
      </head>
      <body> <div id="root"> children </div> </body>
    </html>;
  };
};

let renderPage = path =>
  ReactDOM.renderToStaticMarkup(
    <Page> <App serverUrl={path, hash: "", search: ""} /> </Page>,
  );

Dream.run @@
Dream.logger @@
Dream.router([
  Dream.get("static/**", Dream.static("dist")),
  Dream.get("/", _ => {Dream.html(renderPage([]))}),
  Dream.get("/:name", request => {
    let name = Dream.param(request, "name");
    Dream.html(renderPage([name]));
  }),
]);
