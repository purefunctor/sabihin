module Template = {
  [@react.component]
  let make = (~children) => {
    let dataEmotion =
      CssJs.Stylesheet.get_all(CssJs.instance)
      |> List.map(((name, _)) =>
           String.sub(name, 4, String.length(name) - 4)
         )
      |> List.cons("css")
      |> String.concat(" ");

    let styleTag = {
      React.createElement(
        "style",
        [
          Bool(("data-s", true)),
          String(("data-emotion", dataEmotion)),
          DangerouslyInnerHtml(CssJs.render_style_tag()),
        ],
        [],
      );
    };

    <html lang="en">
      <head>
        <meta charSet="UTF-8" />
        <meta
          name="viewport"
          content="width=device-width, initial-scale=1.0"
        />
        <title> {React.string("Sabihin")} </title>
        <script type_="module" src="http://localhost:5173/@vite/client" />
        <script type_="module" src="http://localhost:5173/index.js" />
        <link rel="stylesheet" href="assets/global.css" />
        styleTag
      </head>
      <body> <div id="root"> children </div> </body>
    </html>;
  };
};

let toString = path => {
  let serverUrl: ReasonReactRouter.url = {path, hash: "", search: ""};
  "<!doctype html>"
  ++ ReactDOM.renderToString(
       <Template>
         <ServerUrl.Provider value={Some(serverUrl)}>
           <App />
         </ServerUrl.Provider>
       </Template>,
     );
};
