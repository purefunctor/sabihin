[@react.component]
let make = (~registerState, ~nextStep) => {
  Js.Console.log("Generate Step");
  Js.Console.log(registerState);
  Js.Console.log(nextStep);
  React.useEffect0(() => {
    let _ = Js.Global.setTimeout(~f=nextStep, 1000);
    None;
  });
  <div> {React.string("Generating Keys")} </div>;
};
