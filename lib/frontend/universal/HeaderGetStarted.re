let getStartedCss = [%cx
  {|
display: flex;
gap: 1rem;
margin-left: auto;

$(Theme.afterSmall) {
  display: grid;
  grid-template-columns: repeat(2, minmax(0, 1fr));
}
|}
];

let loginButtonCss =
  Theme.ghostButton
  ++ " "
  ++ [%cx {|
display: none;
$(Theme.afterSmall) {
  display: flex;
}
|}];

let registerButtonCss = Theme.primaryButton;

[@react.component]
let make = () => {
  <nav className=getStartedCss>
    <Link className=loginButtonCss href="/login">
      {React.string("Login")}
    </Link>
    <Link className=registerButtonCss href="/register">
      {React.string("Register")}
    </Link>
  </nav>;
};
