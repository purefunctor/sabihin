let titleCss = [%cx {|
font-size: 2rem;
font-weight: 700;
|}];

let dotCss = [%cx {|
color: $(Theme.primary);
|}];

[@react.component]
let make = () => {
    <Link className=titleCss href="/">
      {React.string("sabihin")}
      <span className=dotCss> {React.string(".")} </span>
      {React.string("ph")}
    </Link>
};
