// ============================================================================
// 
//       Filename:  Model.cpp
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  04/11/2011 01:35:21 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Neal van Veen (), nealvanveen@gmail.com
//        Company:  
// 
// ============================================================================

#include    <algorithm>
#include    <fstream>
#include    <exception>
#include    "Model.h"

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  Model
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Model ()
{
    // Don't animate anything at the start.
    animIndex_ = -1;
}  // -----  end of method Model::Model  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  Model
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Model ( const std::string& fileName, Context& context, 
               const GLfloat& x, const GLfloat& y, const GLfloat& z,
               Shader& shader ) :
    DrawableInterface(context, shader), fileName_(fileName)
{
    // Read data
    readData();
    // Force updating of matrices
    setMVP(true);
    // Move the model to its location
    move(x*32.0f, y*32.0f, z*32.0f);
    // Don't animate anything at the start.
    animIndex_= -1;
}  // -----  end of method Model::Model  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  ~Model
// Description:  destructor
//-----------------------------------------------------------------------------
Model::~Model ()
{
}  // -----  end of method Model::~Model  (destructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  readData
// Description:  Read the model file and construct the limbs.
//-----------------------------------------------------------------------------
    void
Model::readData ()
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit | 
                    std::ifstream::eofbit);
    try {
        // Open file
        file.open(fileName_.c_str(), std::ios::binary);
        // Read number of limbs
        Byte numLimbs;
        file.read(reinterpret_cast<char *>(&numLimbs), sizeof(Byte));
        limbs_.reserve(numLimbs);

        // Vectors to determine size of the limb-enclosing blocks.
        Vec3 minSide, maxSide;
        minSide.fill(0); maxSide.fill(0);

        // For every limb, read the designated number of voxels and create
        // the appropriate object.
        for ( Uint i = 0; i < numLimbs; i += 1 ) {
            Uint numVoxels;
            Byte temp;
            file.read(reinterpret_cast<char *>(&numVoxels), sizeof(Uint));

            // Read offsets and box sizes.
            Vec3 offset, boxSize;
            for ( Uint j = 0; j < 6; j += 1 ) {
                file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
                if ( j < 3 ) offset[j] = (float)temp;
                else boxSize[j-3] = (float)temp;
            }

            // Number of voxels has been determined, so loop until the end
            // adding every voxel to the limb.
            std::vector<Voxel> voxelList;
            for ( Uint j = 0; j < numVoxels; j += 1 ) {
                Voxel v;
                file.read(reinterpret_cast<char *>(&v.x), sizeof(Byte));
                file.read(reinterpret_cast<char *>(&v.y), sizeof(Byte));
                file.read(reinterpret_cast<char *>(&v.z), sizeof(Byte));
                // Read voxel color.
                for ( Uint k = 0; k < 4; k += 1 ) {
                    file.read(reinterpret_cast<char *>(&temp), sizeof(Byte));
                    v.color[k] = (GLfloat)temp / 255.0f;
                }
                voxelList.push_back(v);
            }
            // Determine the offset
            for ( Uint j = 0; j < 3; j += 1 ) {
                minSide[j] = std::min(offset[j], minSide[j]);
                maxSide[j] = std::max(offset[j]+boxSize[j], maxSide[j]);
                offset[j] -= (maxSide[j]-minSide[j])/2;
            }
            // Add limb to model
            limbs_.emplace_back(voxelList, offset, boxSize);
        }
        file.close();
    }
    catch ( std::ifstream::failure e ) {
        // Catch error
        if ( file.eof() )
            std::cerr << "Early EOF detected in ";
        else if ( file.fail() )
            std::cerr << "Could not read file ";
        else if ( file.bad() )
            std::cerr << "Damaged file ";
        else
            std::cerr << "Unknown error detected in ";
        std::cerr << fileName_ << std::endl << std::endl;
        throw;
    }
}		// -----  end of method Model::readData  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  draw
// Description:  Draw every Limb as a DrawableObject.
//-----------------------------------------------------------------------------
    void
Model::draw ()
{
    // Set OpenGL state.
    this->setDrawState();
    // Loop through limbs and draw.
    for ( Limb& l : limbs_ ) {
        // Begin finding the current animation matrix (animatrix?)
        glm::mat4 texTransform = glm::mat4(1.0f);
        // Are we animating?
        if ( animIndex_ != -1 ) {
            // How far into an animation are we?
            int index = (std::chrono::system_clock::now()-animBegin_).count();
            index /= 50000;
            // Get active animation.
            Animation *anim = &animations_[animIndex_];

            // If we outrun the duration of the animation, do 2 things.
            if ( index >= anim->matrices[&l].size() ) {
                // If we're not looping, stop the animation.
                if ( !anim->loop ) {
                    animIndex_ = -1;
                    index = 0;
                }
                // Otherwise, wrap around.
                else {
                    index %= anim->matrices[&l].size();
                }
            }
            // Set animatrix.
            texTransform = *(anim->matrices[&l].begin()+index);
        }
        // Draw
        shader_->setUniform("texTransform", texTransform);
        l.draw();
    }
    // Clean up
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindVertexArray(0);
}		// -----  end of method Model::draw  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  addAnimation
// Description:  Add a new animation to the list and return its index.
//-----------------------------------------------------------------------------
    Model::Uint
Model::addAnimation ( const bool& loop )
{
    try {
        // Check for faulty previous animations and exit.
        for ( Animation& a : animations_ )
            if ( a.matrices.size() != limbs_.size() ) throw;

        // New animation
        std::map<Limb *, std::vector<glm::mat4>> newAnimation;
        // Add new entry for every limb in the limblist.
        for ( Limb& l : limbs_ ) {
            std::pair<Limb *, std::vector<glm::mat4>> p(&l,
                    std::vector<glm::mat4>(1, glm::mat4(1.0f)));
            auto succeeded = newAnimation.insert(p);
            if ( !succeeded.second ) throw p;
        }
        // Add it.
        animations_.emplace_back(Animation{newAnimation, loop});
        // Return its index.
        return animations_.size()-1;
    }
    catch ( std::pair<Limb *, std::vector<glm::mat4>>& e ) {
        std::cerr << "Failed to insert animation matrices for " << e.first;
        std::cerr << "." << std::endl;
    }
    catch (...) {
        std::cerr << "Something went wrong with adding an animation.";
        std::cerr << std::endl << "Removing all animations to be sure.";
        std::cerr << std::endl;
        throw;
    }
}		// -----  end of method Model::addAnimation  -----

//-----------------------------------------------------------------------------
//       Class:  Model
//      Method:  startAnimation
// Description:  Start counter for animIndex animation.
//-----------------------------------------------------------------------------
    void
Model::startAnimation ( const Uint& animIndex )
{
    try {
        if ( animIndex >= animations_.size() ) throw animIndex;

        animIndex_ = animIndex;
        animBegin_ = std::chrono::system_clock::now();
    }
    catch ( const Uint& i ) {
        std::cerr << "Invalid animation index " << i << "." << std::endl;
    }
}		// -----  end of method Model::startAnimation  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  Model::Limb
// Description:  constructor
//-----------------------------------------------------------------------------
Model::Limb::Limb ( const std::vector<Voxel>& voxels, const Vec3& offset,
                    const Vec3& boxSize ) :
    voxels_(voxels), offset_(offset), boxSize_(boxSize), DrawableObject()
{
    constructed_ = false;
}  // -----  end of method Model::Limb::Model::Limb  (constructor)  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  construct
// Description:  Collector function.
//-----------------------------------------------------------------------------
    void
Model::Limb::construct ()
{
    // If the model/limb is at the first draw call, the data hasn't been
    // constructed yet, so do that first.
    if ( !constructed_ ) {
        createVertexBuffer();
        createTexture();
        constructed_ = true;
    }
}		// -----  end of method Model::Limb::construct  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  addjustBoxSize
// Description:  Calculate how much each animation changes the boxsize,
//               and adjust accordingly.
//-----------------------------------------------------------------------------
    void
Model::Limb::adjustBoxSize ( const std::vector<glm::mat4>& matrices )
{
    Vec3 minSide, maxSide;
    minSide.fill(256); maxSide.fill(0);
    for ( glm::mat4 m : matrices ) {
        for ( Voxel &v : voxels_ ) {
            // Prerun every animatrix, and determine how big the box has 
            // become.
            glm::vec4 pos = glm::vec4(v.x, v.y, v.z, 1.0f);
            pos = m * pos;
            pos = glm::floor(pos);
            for ( Uint i = 0; i < 3; i++ ) {
                minSide[i] = std::min(minSide[i], pos[i]);
                maxSide[i] = std::max(maxSide[i], pos[i]);
            }
        }
    }
    // Set boxsize, with a limitation that it can't be smaller (which might
    // happen after rounding), and it might need some more space (also because
    // of rounding).
    for ( Uint i = 0; i < 3; i++ ) {
        maxSide[i] += 1;
        float diff = maxSide[i] - minSide[i];
        boxSize_[i] = (diff > boxSize_[i]) ? diff : boxSize_[i];
    }
    // Now offset every voxel.
    for ( Voxel &v : voxels_ ) {
        if ( v.x > (Byte)minSide[0] ) v.x -= (Byte)minSide[0];
        if ( v.y > (Byte)minSide[1] ) v.y -= (Byte)minSide[1];
        if ( v.z > (Byte)minSide[2] ) v.z -= (Byte)minSide[2];
    }
}		// -----  end of method Model::Limb::adjustBoxSize  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  createVertexBuffer
// Description:  Method that creates a vertex buffer object from a list of
//               voxels.
//-----------------------------------------------------------------------------
    void
Model::Limb::createVertexBuffer ()
{
    std::vector<GLfloat> data;
    // Temporary vectors for shorthand notation.
    Vec3 b(boxSize_), o(offset_);
    // Add slices on each x,y and z axis on both sides of a voxel.
    for ( GLfloat x = o[0]; x < b[0]+o[0]; x += 1 ) {
        // Variable that determines position of texels in texture.
        GLfloat r = x-o[0]+0.5f;
        std::vector<GLfloat> slice = {
            x, o[1],      o[2]+b[2], r, 0.0f, b[2],
            x, o[1]+b[1], o[2]+b[2], r, b[1], b[2],
            x, o[1]+b[1], o[2],      r, b[1], 0.0f,
            x, o[1],      o[2]+b[2], r, 0.0f, b[2],
            x, o[1]+b[1], o[2],      r, b[1], 0.0f,
            x, o[1],      o[2],      r, 0.0f, 0.0f,

            x+1, o[1],      o[2],      r, 0.0f, 0.0f,
            x+1, o[1]+b[1], o[2],      r, b[1], 0.0f,
            x+1, o[1]+b[1], o[2]+b[2], r, b[1], b[2],
            x+1, o[1],      o[2],      r, 0.0f, 0.0f,
            x+1, o[1]+b[1], o[2]+b[2], r, b[1], b[2],
            x+1, o[1],      o[2]+b[2], r, 0.0f, b[2],
        };
        data.insert(data.end(), slice.begin(), slice.end());
    }
    for ( GLfloat y = o[1]; y < b[1]+o[1]; y += 1 ) {
        GLfloat s = y-o[1]+0.5f;
        std::vector<GLfloat> slice = {
            o[0],      y, o[2]+b[2], 0.0f, s, b[2],
            o[0],      y, o[2],      0.0f, s, 0.0f,
            o[0]+b[0], y, o[2],      b[0], s, 0.0f,
            o[0],      y, o[2]+b[2], 0.0f, s, b[2],
            o[0]+b[0], y, o[2],      b[0], s, 0.0f,
            o[0]+b[0], y, o[2]+b[2], b[0], s, b[2],

            o[0],      y+1, o[2],      0.0f, s, 0.0f,
            o[0],      y+1, o[2]+b[2], 0.0f, s, b[2],
            o[0]+b[0], y+1, o[2]+b[2], b[0], s, b[2],
            o[0],      y+1, o[2],      0.0f, s, 0.0f,
            o[0]+b[0], y+1, o[2]+b[2], b[0], s, b[2],
            o[0]+b[0], y+1, o[2],      b[0], s, 0.0f,
        };
        data.insert(data.end(), slice.begin(), slice.end());
    }
    for ( GLfloat z = o[2]; z < b[2]+o[2]; z += 1 ) {
        GLfloat t = z-o[2]+0.5f;
        std::vector<GLfloat> slice = {
            o[0],      o[1],      z, 0.0f, 0.0f, t,
            o[0],      o[1]+b[1], z, 0.0f, b[1], t,
            o[0]+b[0], o[1]+b[1], z, b[0], b[1], t,
            o[0],      o[1],      z, 0.0f, 0.0f, t,
            o[0]+b[0], o[1]+b[1], z, b[0], b[1], t,
            o[0]+b[0], o[1],      z, b[0], 0.0f, t,

            o[0]+b[0], o[1],      z+1, b[0], 0.0f, t,
            o[0]+b[0], o[1]+b[1], z+1, b[0], b[1], t,
            o[0],      o[1]+b[1], z+1, 0.0f, b[1], t,
            o[0]+b[0], o[1],      z+1, b[0], 0.0f, t,
            o[0],      o[1]+b[1], z+1, 0.0f, b[1], t,
            o[0],      o[1],      z+1, 0.0f, 0.0f, t,
        };
        data.insert(data.end(), slice.begin(), slice.end());
    }
    // Add the data to create the object.
    this->itemCount_ = data.size()/6;
    this->DrawableObject::createVertexBuffer(&data[0]);
}		// -----  end of method Model::Limb::createVertexBuffer  -----

//-----------------------------------------------------------------------------
//       Class:  Model::Limb
//      Method:  createTexture
// Description:  Create a texture from the voxel information that is stored.
//-----------------------------------------------------------------------------
    void
Model::Limb::createTexture ()
{
    // Buffer that holds the texture information
    std::vector<glm::vec4> texMap(boxSize_[0]*boxSize_[1]*boxSize_[2],
                                  glm::vec4(0));
    for ( const Voxel &v : voxels_ ) {
        // Since texMap is a 1d array, we also need a 3D index into that array.
        Uint mapIndex = v.z*boxSize_[0]*boxSize_[1]+v.y*boxSize_[0]+v.x;
        texMap[mapIndex] = v.color;
    }
    // Pass the appropriate buffer and size data to create the texture.
    this->DrawableObject::createTexture(&texMap[0][0], 
                                        boxSize_[0], boxSize_[1], boxSize_[2]);
}		// -----  end of method Model::Limb::createTexture  -----

    void
Model::Limb::draw ()
{
    construct();
    this->DrawableObject::draw();
}		// -----  end of method Model::Limb::draw  -----

