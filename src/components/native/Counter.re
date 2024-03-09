[@react.component]
let make = (~name) => {
  let (count, setCount) = React.useState(() => 0);
  <div>
    {React.string(
       name ++ " has clicked " ++ Belt.Int.toString(count) ++ " times.",
     )}
    <button onClick={_ => setCount(count => count + 1)}>
      {React.string("Click Me!")}
    </button>
  </div>;
};
