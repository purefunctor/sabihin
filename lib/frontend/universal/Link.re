[@react.component]
let make = (~className=?, ~onClick=_ => (), ~href, ~children=React.null) => {
  let onClick = event => {
    event |> React.Event.Mouse.preventDefault;
    onClick();
    ReasonReactRouter.push(href);
  };
  <a ?className href onClick> children </a>;
};
