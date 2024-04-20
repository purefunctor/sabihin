[@react.component]
let make = () => {
  <main className="profile-content">
    <div className="profile-header">
      <img
        className="profile-photo"
        src="https://avatars.githubusercontent.com/u/66708316?v=4"
        height="96px"
        width="96px"
      />
      <div className="profile-info">
        <ProfileUsernameButton />
        <p className="poppins-regular profile-public-id">
          {React.string("Public ID: publicid1234")}
        </p>
      </div>
    </div>
  </main>;
};
