[@react.component]
let make = (~className=?, ~href, ~children) => {
  let onClick = event => {
    event |> React.Event.Mouse.preventDefault;
    ReasonReactRouter.push(href);
  };
  <a ?className href onClick> children </a>;
};
