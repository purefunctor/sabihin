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

let loginButtonCss = [%cx
  {|
display: none;

border: 1px solid $(Theme.backgroundSubtle);
border-radius: 8px;
padding: 1rem;

$(Theme.afterSmall) {
  display: flex;
  align-items: center;
  justify-content: center;
}
|}
];

let registerButtonCss = [%cx
  {|
display: flex;
align-items: center;
justify-content: center;

color: $(Theme.background);
background-color: $(Theme.primary);
border: 1px solid $(Theme.primary);
border-radius: 8px;
padding: 1rem;
|}
];

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
