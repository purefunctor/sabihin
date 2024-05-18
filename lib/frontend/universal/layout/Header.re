let headerCss = [%cx
  {|
font-family: "Josefin Sans";
display: flex;
align-items: center;

height: 4rem;
border-bottom: 1px solid $(Theme.backgroundSubtle);
padding-top: 1rem;
padding-bottom: 1rem;
|}
];

let titleCss = [%cx {|
font-size: 2rem;
font-weight: 700;
|}];

let dotCss = [%cx {|
color: $(Theme.primary);
|}];

[@react.component]
let make = () => {
  <header className=headerCss>
    <Link className=titleCss href="/">
      {React.string("sabihin")}
      <span className=dotCss> {React.string(".")} </span>
      {React.string("ph")}
    </Link>
    <HeaderNav />
  </header>;
};
