open InboxPageMessagesStyles;
open InboxPageMessagesCardHooks;

module Face = {
  [@react.component]
  let make = (~faceInitial, ~faceAnimate, ~toggleReveal) => {
    let onClick = React.useCallback1(_ => toggleReveal(), [|toggleReveal|]);
    <FramerMotion.div
      className=cardFaceCss
      variants=cardVariants
      initial=faceInitial
      animate=faceAnimate>
      <button className=cardFaceCloseCss onClick>
        <Icons.CloseLine size="2rem" />
      </button>
      <div className=cardFaceContentCss>
        {React.string("example content :3")}
      </div>
      <button className=cardFaceTrashCss>
        <Icons.DeleteBinLine size="1.2rem" />
        <span> {React.string("Trash")} </span>
      </button>
      <button className=cardFaceReplyCss>
        <Icons.ChatLine size="1.2rem" />
        <span> {React.string("Reply")} </span>
      </button>
      <button className=cardFaceShareCss>
        <Icons.LinkM size="1.2rem" />
        <span> {React.string("Share")} </span>
      </button>
    </FramerMotion.div>;
  };
};

module Back = {
  [@react.component]
  let make = (~backInitial, ~backAnimate, ~toggleReveal) => {
    let onClick = React.useCallback1(_ => toggleReveal(), [|toggleReveal|]);
    <FramerMotion.div
      className=cardBackCss
      variants=cardVariants
      initial=backInitial
      animate=backAnimate
      onClick>
      <Icons.MailLockLine size="8rem" />
    </FramerMotion.div>;
  };
};

[@react.component]
let make = () => {
  let (faceInitial, faceAnimate, backInitial, backAnimate, toggleReveal) =
    useRevealState();

  <div className=cardOuterCss>
    <Face faceInitial faceAnimate toggleReveal />
    <Back backInitial backAnimate toggleReveal />
  </div>;
};
