module Menu = {
  let menuCss = [%cx
    {|
    position: absolute;
    background-color: $(Theme.background11);
    border-color: $(Theme.background9);
    border-style: solid;
    border-width: 1px;
    top: 100%;
    right: 0;
    margin-top: calc(1rem + 2px);

    display: flex;
    flex-direction: column;
    padding: 2rem;
    gap: 2rem;
    font-size: 1.1rem;
    font-family: "Poppins";
  |}
  ];

  let linkCss = [%cx
    {|
    display: flex;
    align-items: center;
    gap: 1rem;
  |}
  ];

  [@react.component]
  let make = (~onLinkClick) => {
    <nav className=menuCss>
      <Link href="/profile" className=linkCss onClick=onLinkClick>
        <Icons.UserLine />
        {React.string("Profile")}
      </Link>
      <Link href="/inbox" className=linkCss onClick=onLinkClick>
        <Icons.InboxLine />
        {React.string("Inbox")}
      </Link>
      <Link href="/write" className=linkCss onClick=onLinkClick>
        <Icons.QuillLine />
        {React.string("Write")}
      </Link>
      <Link href="/learn" className=linkCss onClick=onLinkClick>
        <Icons.QuestionLink />
        {React.string("Learn")}
      </Link>
    </nav>;
  };
};

let smallGroupCss = [%cx
  {|
  display: flex;
  $(Theme.afterMedium) {
    display: none;
  }
  position: relative;
|}
];

let menuButtonCss =
  Theme.ghostButton ++ " " ++ [%cx {| color: $(Theme.foreground10); |}];

[@react.component]
let make = () => {
  let (toggled, setToggled) = React.useState(() => false);
  let onClick =
    React.useCallback1(
      _ => {setToggled(prevToggled => !prevToggled)},
      [|setToggled|],
    );

  let onLinkClick =
    React.useCallback1(_ => {setToggled(_ => false)}, [|setToggled|]);

  <div className=smallGroupCss>
    <button className=menuButtonCss onClick>
      <Icons.MenuLine size="1.5rem" />
    </button>
    {toggled ? <Menu onLinkClick /> : React.null}
  </div>;
};
