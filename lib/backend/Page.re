module Template = {
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
        {Vite.make_script_tags()}
        <link rel="stylesheet" href="/static/fonts.css" />
        <link rel="stylesheet" href="/static/global.css" />
        {CssJs.style_tag()}
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
